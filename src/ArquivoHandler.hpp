#ifndef ARQUIVO
#define ARQUIVO

#include <fstream>
#include "vector"
#include "string"

class ArquivoHandler {
public:
    virtual void getLine(std::string* linha) = 0;
    virtual void writeLine(std::string linha) = 0;
    virtual bool hasEnd() = 0;
    virtual void resetFile() = 0;
    virtual void finishWrite() {};
};

class ArquivoEmMemoria : public ArquivoHandler {
public:
    int linhaAtual;
    std::vector<std::string> arquivo;
    void getLine(std::string *linha) override;
    void writeLine(std::string linha) override;
    bool hasEnd() override;
    void resetFile() override;
};

class ArquivoFisico : public ArquivoHandler {
public:
    std::fstream arquivo;
    explicit ArquivoFisico(const char *nomeArquivo);
    void getLine(std::string *linha) override;
    void writeLine(std::string linha) override;
    bool hasEnd() override;
    void resetFile() override;
    void finishWrite() override;
};

#endif