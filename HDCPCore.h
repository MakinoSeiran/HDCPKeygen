#pragma once
#include <vector>
#include <string>
#include <cstdint>

// 56-bit mask (2^56 - 1)
const uint64_t MASK_56 = 0x00FFFFFFFFFFFFFFULL;

class HDCPCore
{
public:
    HDCPCore();
    virtual ~HDCPCore();

    // Load master key matrix from file
    bool LoadMasterKey(const std::wstring& filePath);

    // Generate random KSV (40 bits, 20 ones, 20 zeros)
    uint64_t GenerateKSV() const;

    // Generate Source Key from KSV (returns a vector of 40 56-bit numbers)
    std::vector<uint64_t> GenerateSourceKey(uint64_t ksv) const;

    // Generate Sink Key from KSV (using transposed matrix)
    std::vector<uint64_t> GenerateSinkKey(uint64_t ksv) const;

    // Self-test: generate a random Source/Sink pair and verify shared keys are equal
    bool SelfTest(std::string& outMessage) const;

    // Format KSV as a 10-digit hexadecimal string
    static std::string FormatKSV(uint64_t ksv);

    // Format Key vector as 40 14-digit hexadecimal strings (5 per line)
    static std::string FormatKey(const std::vector<uint64_t>& key, bool isSink);

    // Get last error message
    std::string GetLastError() const { return m_lastError; }

private:
    std::vector<std::vector<uint64_t>> m_masterMatrix;      // Original matrix 40x40
    std::vector<std::vector<uint64_t>> m_transposedMatrix;  // Transposed matrix
    mutable std::string m_lastError;

    // Transpose matrix
    void TransposeMatrix();

    // Core generation logic: generate Key from KSV and matrix
    std::vector<uint64_t> GenerateKeyFromMatrix(uint64_t ksv, const std::vector<std::vector<uint64_t>>& matrix) const;
};