#include "UnistdError.h"

UnistdError::UnistdError(const std::string &message, int error)
    : message(message + ": ")
{
    switch (error)
    {
    case E2BIG:
        this->message += "Argument list too long (POSIX.1)";
        break;
    case EACCES:
        this->message += "Permission denied (POSIX.1)";
        break;
    case EADDRINUSE:
        this->message += "Address already in use (POSIX.1)";
        break;
    case EADDRNOTAVAIL:
        this->message += "Address not available (POSIX.1)";
        break;
    case EAFNOSUPPORT:
        this->message += "Address family not supported (POSIX.1)";
        break;
    case EAGAIN:
        this->message += "Resource  temporarily  unavailable (may be the same value as EWOULD‐BLOCK) (POSIX.1)";
        break;
    case EALREADY:
        this->message += "Connection already in progress (POSIX.1)";
        break;
    case EBADE:
        this->message += "Invalid exchange";
        break;
    case EBADF:
        this->message += "Bad file descriptor (POSIX.1)";
        break;
    case EBADFD:
        this->message += "File descriptor in bad state";
        break;
    case EBADMSG:
        this->message += "Bad message (POSIX.1)";
        break;
    case EBADR:
        this->message += "Invalid request descriptor";
        break;
    case EBADRQC:
        this->message += "Invalid request code";
        break;
    case EBADSLT:
        this->message += "Invalid slot";
        break;
    case EBUSY:
        this->message += "Device or resource busy (POSIX.1)";
        break;
    case ECANCELED:
        this->message += "Operation canceled (POSIX.1)";
        break;
    case ECHILD:
        this->message += "No child processes (POSIX.1)";
        break;
    case ECHRNG:
        this->message += "Channel number out of range";
        break;
    case ECOMM:
        this->message += "Communication error on send";
        break;
    case ECONNABORTED:
        this->message += "Connection aborted (POSIX.1)";
        break;
    case ECONNREFUSED:
        this->message += "Connection refused (POSIX.1)";
        break;
    case ECONNRESET:
        this->message += "Connection reset (POSIX.1)";
        break;
    case EDEADLK:
        this->message += "Resource deadlock avoided (POSIX.1)";
        break;
    case EDESTADDRREQ:
        this->message += "Destination address required (POSIX.1)";
        break;
    case EDOM:
        this->message += "Mathematics argument out of domain of function (POSIX.1, C99)";
        break;
    case EDQUOT:
        this->message += "Disk quota exceeded (POSIX.1)";
        break;
    case EEXIST:
        this->message += "File exists (POSIX.1)";
        break;
    case EFAULT:
        this->message += "Bad address (POSIX.1)";
        break;
    case EFBIG:
        this->message += "File too large (POSIX.1)";
        break;
    case EHOSTDOWN:
        this->message += "Host is down";
        break;
    case EHOSTUNREACH:
        this->message += "Host is unreachable (POSIX.1)";
        break;
    case EIDRM:
        this->message += "Identifier removed (POSIX.1)";
        break;
    case EILSEQ:
        this->message += "Illegal byte sequence (POSIX.1, C99)";
        break;
    case EINPROGRESS:
        this->message += "Operation in progress (POSIX.1)";
        break;
    case EINTR:
        this->message += "Interrupted function call (POSIX.1); see signal(7).";
        break;
    case EINVAL:
        this->message += "Invalid argument (POSIX.1)";
        break;
    case EIO:
        this->message += "Input/output error (POSIX.1)";
        break;
    case EISCONN:
        this->message += "Socket is connected (POSIX.1)";
        break;
    case EISDIR:
        this->message += "Is a directory (POSIX.1)";
        break;
    case EISNAM:
        this->message += "Is a named type file";
        break;
    case EKEYEXPIRED:
        this->message += "Key has expired";
        break;
    case EKEYREJECTED:
        this->message += "Key was rejected by service";
        break;
    case EKEYREVOKED:
        this->message += "Key has been revoked";
        break;
    case EL2HLT:
        this->message += "Level 2 halted";
        break;
    case EL2NSYNC:
        this->message += "Level 2 not synchronized";
        break;
    case EL3HLT:
        this->message += "Level 3 halted";
        break;
    case EL3RST:
        this->message += "Level 3 halted";
        break;
    case ELIBACC:
        this->message += "Cannot access a needed shared library";
        break;
    case ELIBBAD:
        this->message += "Accessing a corrupted shared library";
        break;
    case ELIBMAX:
        this->message += "Attempting to link in too many shared libraries";
        break;
    case ELIBSCN:
        this->message += "lib section in a.out corrupted";
        break;
    case ELIBEXEC:
        this->message += "Cannot exec a shared library directly";
        break;
    case ELOOP:
        this->message += "Too many levels of symbolic links (POSIX.1)";
        break;
    case EMEDIUMTYPE:
        this->message += "Wrong medium type";
        break;
    case EMFILE:
        this->message += "Too many open files (POSIX.1)";
        break;
    case EMLINK:
        this->message += "Too many links (POSIX.1)";
        break;
    case EMSGSIZE:
        this->message += "Message too long (POSIX.1)";
        break;
    case EMULTIHOP:
        this->message += "Multihop attempted (POSIX.1)";
        break;
    case ENAMETOOLONG:
        this->message += "Filename too long (POSIX.1)";
        break;
    case ENETDOWN:
        this->message += "Network is down (POSIX.1)";
        break;
    case ENETRESET:
        this->message += "Connection aborted by network (POSIX.1)";
        break;
    case ENETUNREACH:
        this->message += "Network unreachable (POSIX.1)";
        break;
    case ENFILE:
        this->message += "Too many open files in system (POSIX.1)";
        break;
    case ENOBUFS:
        this->message += "No buffer space available (POSIX.1 (XSI STREAMS option))";
        break;
    case ENODATA:
        this->message += "No message is available on the STREAM head read queue (POSIX.1)";
        break;
    case ENODEV:
        this->message += "No such device (POSIX.1)";
        break;
    case ENOENT:
        this->message += "No such file or directory (POSIX.1)";
        break;
    case ENOEXEC:
        this->message += "Exec format error (POSIX.1)";
        break;
    case ENOKEY:
        this->message += "Required key not available";
        break;
    case ENOLCK:
        this->message += "No locks available (POSIX.1)";
        break;
    case ENOLINK:
        this->message += "Link has been severed (POSIX.1)";
        break;
    case ENOMEDIUM:
        this->message += "No medium found";
        break;
    case ENOMEM:
        this->message += "Not enough space (POSIX.1)";
        break;
    case ENOMSG:
        this->message += "No message of the desired type (POSIX.1)";
        break;
    case ENONET:
        this->message += "Machine is not on the network";
        break;
    case ENOPKG:
        this->message += "Package not installed";
        break;
    case ENOPROTOOPT:
        this->message += "Protocol not available (POSIX.1)";
        break;
    case ENOSPC:
        this->message += "No space left on device (POSIX.1)";
        break;
    case ENOSR:
        this->message += "No STREAM resources (POSIX.1 (XSI STREAMS option))";
        break;
    case ENOSTR:
        this->message += "Not a STREAM (POSIX.1 (XSI STREAMS option))";
        break;
    case ENOSYS:
        this->message += "Function not implemented (POSIX.1)";
        break;
    case ENOTBLK:
        this->message += "Block device required";
        break;
    case ENOTCONN:
        this->message += "The socket is not connected (POSIX.1)";
        break;
    case ENOTDIR:
        this->message += "Not a directory (POSIX.1)";
        break;
    case ENOTEMPTY:
        this->message += "Directory not empty (POSIX.1)";
        break;
    case ENOTSOCK:
        this->message += "Not a socket (POSIX.1)";
        break;
    case ENOTSUP:
        this->message += "Operation not supported (POSIX.1)";
        break;
    case ENOTTY:
        this->message += "Inappropriate I/O control operation (POSIX.1)";
        break;
    case ENOTUNIQ:
        this->message += "Name not unique on network";
        break;
    case ENXIO:
        this->message += "No such device or address (POSIX.1)";
        break;
    case EOVERFLOW:
        this->message += "Value too large to be stored in data type (POSIX.1)";
        break;
    case EPERM:
        this->message += "Operation not permitted (POSIX.1)";
        break;
    case EPFNOSUPPORT:
        this->message += "Protocol family not supported";
        break;
    case EPIPE:
        this->message += "Broken pipe (POSIX.1)";
        break;
    case EPROTO:
        this->message += "Protocol error (POSIX.1)";
        break;
    case EPROTONOSUPPORT:
        this->message += "Protocol not supported (POSIX.1)";
        break;
    case EPROTOTYPE:
        this->message += "Protocol wrong type for socket (POSIX.1)";
        break;
    case ERANGE:
        this->message += "Result too large (POSIX.1, C99)";
        break;
    case EREMCHG:
        this->message += "Remote address changed";
        break;
    case EREMOTE:
        this->message += "Object is remote";
        break;
    case EREMOTEIO:
        this->message += "Remote I/O error";
        break;
    case ERESTART:
        this->message += "Interrupted system call should be restarted";
        break;
    case EROFS:
        this->message += "Read-only filesystem (POSIX.1)";
        break;
    case ESHUTDOWN:
        this->message += "Cannot send after transport endpoint shutdown";
        break;
    case ESPIPE:
        this->message += "Invalid seek (POSIX.1)";
        break;
    case ESOCKTNOSUPPORT:
        this->message += "Socket type not supported";
        break;
    case ESRCH:
        this->message += "No such process (POSIX.1)";
        break;
    case ESTALE:
        this->message += "Stale file handle (POSIX.1)";
        break;
    case ESTRPIPE:
        this->message += "Streams pipe error";
        break;
    case ETIME:
        this->message += "Timer expired (POSIX.1 (XSI STREAMS option))";
        break;
    case ETIMEDOUT:
        this->message += "Connection timed out (POSIX.1)";
        break;
    case ETXTBSY:
        this->message += "Text file busy (POSIX.1)";
        break;
    case EUCLEAN:
        this->message += "Structure needs cleaning";
        break;
    case EUNATCH:
        this->message += "Protocol driver not attached";
        break;
    case EUSERS:
        this->message += "Too many users";
        break;
    case EXDEV:
        this->message += "Improper link (POSIX.1)";
        break;
    case EXFULL:
        this->message += "Exchange full";
        break;
    default:
        this->message += "Unknown error";
        break;
    }
}

const char * UnistdError::what() const noexcept
{
    return message.c_str();
}
