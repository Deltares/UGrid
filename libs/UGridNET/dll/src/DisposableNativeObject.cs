using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace UGridNET
{
    /// <summary>
    /// Base class for disposable mesh objects. Provides pinning of arrays in memory for exchange with native calls.
    /// </summary>
    public abstract class DisposableNativeObject<TNative> : IDisposable where TNative : new()
    {
        private readonly Dictionary<object, GCHandle> objectGarbageCollectHandles = new Dictionary<object, GCHandle>();
        private bool disposed;

        /// <summary>
        /// Disposes the unmanaged resources
        /// </summary>
        ~DisposableNativeObject()
        {
            Dispose(false);
        }

        /// <summary>
        /// Indicates if arrays are pinned in memory
        /// </summary>
        private bool IsMemoryPinned
        {
            get { return objectGarbageCollectHandles.Count > 0; }
        }

        /// <inheritdoc/>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Creates a native structure for this object
        /// </summary>
        /// <returns></returns>
        public TNative CreateNativeObject()
        {
            if (!IsMemoryPinned)
            {
                PinMemory();
            }

            var nativeObject = new TNative();
            SetNativeObject(ref nativeObject);
            return nativeObject;
        }

        /// <summary>
        /// Maps the <typeparamref name="TNative"/> object with current state (used in <see cref="CreateNativeObject"/>)
        /// </summary>
        /// <param name="nativeObject">Newly created native object</param>
        protected abstract void SetNativeObject(ref TNative nativeObject);

        /// <summary>
        /// Get the pointer to the pinned object
        /// </summary>
        /// <param name="objectToLookUp">Object to get </param>
        /// <returns></returns>
        protected IntPtr GetPinnedObjectPointer(object objectToLookUp)
        {
            return (objectToLookUp != null) 
                ? objectGarbageCollectHandles[objectToLookUp].AddrOfPinnedObject()
                : IntPtr.Zero;
        }

        /// <summary>
        /// Disposes resources
        /// </summary>
        /// <param name="disposing">
        /// Boolean indicating that the call is called from the dispose method
        /// not the destructor
        /// </param>
        protected virtual void Dispose(bool disposing)
        {
            if (disposed)
            {
                return;
            }

            ReleaseUnmanagedResources();

            disposed = true;
        }

        /// <summary>
        /// Pins the arrays in memory (no garbage collect until unpinned (done in dispose))
        /// </summary>
        private void PinMemory()
        {
            IEnumerable<PropertyInfo> arrayProperties = GetType().GetProperties().Where(f => f.PropertyType.IsArray);

            // force initialization
            foreach (PropertyInfo arrayProperty in arrayProperties)
            {
                Type elementType = arrayProperty.PropertyType.GetElementType();
                object objectToPin = arrayProperty.GetValue(this);
                AddObjectToPin(objectToPin);
            }
        }

        private void UnPinMemory()
        {
            foreach (KeyValuePair<object, GCHandle> valuePair in objectGarbageCollectHandles)
            {
                valuePair.Value.Free();
            }

            objectGarbageCollectHandles.Clear();
        }

         private void AddObjectToPin(object objectToPin)
        {
            if(objectToPin != null)
            {
                objectGarbageCollectHandles.Add(objectToPin, GCHandle.Alloc(objectToPin, GCHandleType.Pinned));
            }
        }

        private void ReleaseUnmanagedResources()
        {
            UnPinMemory();
        }
    }
}