using System;

namespace UGridNET
{
    /// <summary>
    /// Custom exceptions for errors that occur within the UGridNET library.
    /// </summary>
    public sealed class UGridNETException : Exception
    {
        private static readonly string prefix = "UGridNET error";

        /// <summary>
        /// Initializes a new instance of the <see cref="UGridNETException"/> class.
        /// </summary>
        public UGridNETException()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UGridNETException"/> class with a specified error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        public UGridNETException(string message)
            : base($"{prefix}: {message}")
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UGridNETException"/> class with a
        /// specified formatted error message and format arguments.
        /// </summary>
        /// <param name="format">The error message format string.</param>
        /// <param name="args">An array of objects to format.</param>
        public UGridNETException(string format, params object[] args)
            : base(string.Format($"{prefix}: {format}", args))
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UGridNETException"/> class with a
        /// specified error message and a reference to the inner exception that is the cause of this exception.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        /// <param name="innerException">The exception that is the cause of the current exception.</param>
        public UGridNETException(string message, Exception innerException)
            : base($"{prefix}: {message}", innerException)
        {
        }
    }
}