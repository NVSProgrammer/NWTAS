import ctypes

# Define the DataBuffer struct
class DataBuffer(ctypes.Structure):
    _fields_ = [("data", ctypes.POINTER(ctypes.c_ubyte)),
                ("size", ctypes.c_int)]

# Load the DLL
nwtas = ctypes.CDLL('D:\\My_Programs\\Git\\WorkOn\\DLLS\\NWTAS\\x64\\Debug\\NWTAS.dll')

# Define the argument and return types
nwtas.removeComments.argtypes = [ctypes.c_char_p]
nwtas.removeComments.restype = ctypes.c_char_p

nwtas.Tokens.argtypes = [ctypes.c_char_p]
nwtas.Tokens.restype = ctypes.c_char_p

nwtas.Build.argtypes = [ctypes.c_char_p]
nwtas.Build.restype = DataBuffer

# Call the functions
src_path = b"D:\\My_Programs\\Git\\WorkOn\\DLLS\\NWTAS\\test\\"
result = nwtas.Build(src_path)

# Check if result.data is not None
if result.data:
    # Convert the result to a Python list
    output = [result.data[i] for i in range(result.size)]
    
    # Free the allocated memory
    #ctypes.windll.kernel32.HeapFree(ctypes.windll.kernel32.GetProcessHeap(), 0, result.data)

    with open("D:\\My_Programs\\Git\\WorkOn\\DLLS\\NWTAS\\test\\output.nwtb", "wb") as f:
        f.write(bytearray(output))
        print(output)
else:
    print("Error: result.data is None")

# Ensure the string is encoded as bytes
tokens_result = nwtas.Tokens(b'.global\n.bytes 8\n\nSet ~vmode, text\nNew #num, *int:7\n\nNew @draw_num\nDraw text, #num\nRefresh screen\nEnd')
print(tokens_result)
