#ifndef MIPSIM_IO_VGA_H_
#define MIPSIM_IO_VGA_H_

#include <vector>
#include <SDL2/SDL.h>

#include <io/io.h>

class VGA : public IOInterface {
public:
    VGA() : window_(nullptr), renderer_(nullptr),
            screen_(nullptr) {
        vram_.resize(kScreenWidth * kScreenHeight * 4);
    }
    ~VGA() { ReleaseSDL(); }

    bool Refresh();
    void Quit();

    std::uint8_t ReadByte(std::uint32_t addr) override;
    void WriteByte(std::uint32_t addr, std::uint8_t value) override;
    std::uint16_t ReadHalf(std::uint32_t addr) override;
    void WriteHalf(std::uint32_t addr, std::uint16_t value) override;
    std::uint32_t ReadWord(std::uint32_t addr) override;
    void WriteWord(std::uint32_t addr, std::uint32_t value) override;

    std::size_t vram_size() const { return vram_.size(); }

private:
    const int kScreenWidth = 640;
    const int kScreenHeight = 480;

    void InitializeSDL();
    void ReleaseSDL();

    SDL_Window *window_;
    SDL_Renderer *renderer_;
    SDL_Texture *screen_;
    std::vector<std::uint8_t> vram_;
};

#endif // MIPSIM_IO_VGA_H_
