#ifndef _COMPRESSION_H
#define _COMPRESSION_H

#include <vector>

namespace Compression
{
    // Función para comprimir datos
    std::vector<char> Compress(const std::vector<char>& data);

    // Función para descomprimir datos
    std::vector<char> Decompress(const std::vector<char>& compressedData);
}

#endif // COMPRESSION_H
