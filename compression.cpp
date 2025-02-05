#include "Compression.h"
#include <zlib/zlib.h>
#include <stdexcept>

namespace Compression
{
    std::vector<char> Compress(const std::vector<char>& data)
    {
        if (data.empty()) {
            return {};
        }

        uLongf compressedSize = compressBound(data.size());
        std::vector<char> compressedData(compressedSize);

        int result = compress(reinterpret_cast<Bytef*>(compressedData.data()), &compressedSize,
            reinterpret_cast<const Bytef*>(data.data()), data.size());

        if (result != Z_OK) {
            throw std::runtime_error("Compression failed");
        }

        compressedData.resize(compressedSize);
        return compressedData;
    }

    std::vector<char> Decompress(const std::vector<char>& compressedData)
    {
        if (compressedData.empty()) {
            return {};
        }

        uLongf decompressedSize = compressedData.size() * 4; // Estimación inicial del tamaño descomprimido
        std::vector<char> decompressedData(decompressedSize);

        int result = uncompress(reinterpret_cast<Bytef*>(decompressedData.data()), &decompressedSize,
            reinterpret_cast<const Bytef*>(compressedData.data()), compressedData.size());

        while (result == Z_BUF_ERROR) {
            // Si el buffer no es suficientemente grande, incrementar el tamaño
            decompressedSize *= 2;
            decompressedData.resize(decompressedSize);
            result = uncompress(reinterpret_cast<Bytef*>(decompressedData.data()), &decompressedSize,
                reinterpret_cast<const Bytef*>(compressedData.data()), compressedData.size());
        }

        if (result != Z_OK) {
            throw std::runtime_error("Decompression failed");
        }

        decompressedData.resize(decompressedSize);
        return decompressedData;
    }
}
