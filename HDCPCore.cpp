#include "pch.h"
#include "HDCPCore.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <cctype>

HDCPCore::HDCPCore()
{
}

HDCPCore::~HDCPCore()
{
}

bool HDCPCore::LoadMasterKey(const std::wstring& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        m_lastError = "Cannot open master key file.";
        return false;
    }

    m_masterMatrix.clear();
    std::string line;
    int rowCount = 0;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string token;
        std::vector<uint64_t> row;
        while (iss >> token)
        {
            try {
                uint64_t val = std::stoull(token, nullptr, 16);
                row.push_back(val);
            }
            catch (const std::exception&) {
                m_lastError = "Invalid hex number: " + token;
                return false;
            }
        }
        if (!row.empty())
        {
            m_masterMatrix.push_back(row);
            rowCount++;
        }
    }
    file.close();

    if (rowCount != 40)
    {
        m_lastError = "Master key matrix must have 40 rows, but found " + std::to_string(rowCount) + " rows.";
        return false;
    }

    for (int i = 0; i < 40; ++i)
    {
        if (m_masterMatrix[i].size() != 40)
        {
            m_lastError = "Row " + std::to_string(i) + " does not have 40 columns. It has " + std::to_string(m_masterMatrix[i].size()) + " columns.";
            return false;
        }
    }

    TransposeMatrix();
    return true;
}

void HDCPCore::TransposeMatrix()
{
    m_transposedMatrix.resize(40, std::vector<uint64_t>(40));
    for (int i = 0; i < 40; ++i)
        for (int j = 0; j < 40; ++j)
            m_transposedMatrix[j][i] = m_masterMatrix[i][j];
}

uint64_t HDCPCore::GenerateKSV() const
{
    std::vector<int> bits(40);
    std::fill(bits.begin(), bits.begin() + 20, 1);
    std::fill(bits.begin() + 20, bits.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(bits.begin(), bits.end(), g);

    uint64_t ksv = 0;
    for (int i = 0; i < 40; ++i)
    {
        if (bits[i])
            ksv |= (1ULL << i);
    }
    return ksv;
}

std::vector<uint64_t> HDCPCore::GenerateKeyFromMatrix(uint64_t ksv, const std::vector<std::vector<uint64_t>>& matrix) const
{
    std::vector<uint64_t> key(40, 0);
    for (int row = 0; row < 40; ++row)
    {
        if (ksv & (1ULL << row))
        {
            for (int col = 0; col < 40; ++col)
            {
                key[col] = (key[col] + matrix[row][col]) & MASK_56;
            }
        }
    }
    return key;
}

std::vector<uint64_t> HDCPCore::GenerateSourceKey(uint64_t ksv) const
{
    return GenerateKeyFromMatrix(ksv, m_masterMatrix);
}

std::vector<uint64_t> HDCPCore::GenerateSinkKey(uint64_t ksv) const
{
    return GenerateKeyFromMatrix(ksv, m_transposedMatrix);
}

bool HDCPCore::SelfTest(std::string& outMessage) const
{
    uint64_t srcKSV = GenerateKSV();
    uint64_t snkKSV = GenerateKSV();

    auto srcKey = GenerateSourceKey(srcKSV);
    auto snkKey = GenerateSinkKey(snkKSV);

    uint64_t key1 = 0, key2 = 0;
    for (int i = 0; i < 40; ++i)
    {
        if (srcKSV & (1ULL << i))
            key1 = (key1 + snkKey[i]) & MASK_56;
        if (snkKSV & (1ULL << i))
            key2 = (key2 + srcKey[i]) & MASK_56;
    }

    std::ostringstream oss;
    oss << "Source KSV: " << FormatKSV(srcKSV) << "\n"
        << "Sink KSV  : " << FormatKSV(snkKSV) << "\n"
        << "Key1 (src): " << std::hex << std::setw(14) << std::setfill('0') << key1 << "\n"
        << "Key2 (snk): " << std::hex << std::setw(14) << std::setfill('0') << key2 << "\n"
        << (key1 == key2 ? "Test PASSED" : "Test FAILED");
    outMessage = oss.str();

    return (key1 == key2);
}

std::string HDCPCore::FormatKSV(uint64_t ksv)
{
    std::ostringstream oss;
    oss << std::hex << std::setw(10) << std::setfill('0') << (ksv & 0xFFFFFFFFFFULL);
    return oss.str();
}

std::string HDCPCore::FormatKey(const std::vector<uint64_t>& key, bool isSink)
{
    std::ostringstream oss;
    oss << (isSink ? "Sink Key:\n" : "Source Key:\n");
    for (int i = 0; i < 40; i += 5)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (i + j < 40)
                oss << std::hex << std::setw(14) << std::setfill('0') << key[i + j] << " ";
        }
        oss << "\n";
    }
    return oss.str();
}
