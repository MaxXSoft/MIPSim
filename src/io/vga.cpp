#include <io/vga.h>

#include <cstdlib>
#include <cassert>

void VGA::InitializeSDL() {
    if (window_) return;
    // initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    std::atexit(SDL_Quit);
    // create window
    window_ = SDL_CreateWindow(
        "MIPSim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        kScreenWidth, kScreenHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );
    // create renderer
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    // create pixel buffer
    screen_ = SDL_CreateTexture(
        renderer_, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, kScreenWidth, kScreenHeight
    );
}

void VGA::ReleaseSDL() {
    if (window_) {
        SDL_DestroyTexture(screen_);
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
        window_ = nullptr;
        renderer_ = nullptr;
        screen_ = nullptr;
    }
}

bool VGA::Refresh() {
    if (!window_) return false;
    // handle event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN
                && event.key.keysym.sym == SDLK_ESCAPE)) {
            return false;
        }
    }
    // redraw
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, screen_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
    // delay
    SDL_Delay(5);
    return true;
}

void VGA::Quit() {
    ReleaseSDL();
}

std::uint8_t VGA::ReadByte(std::uint32_t addr) {
    if (!window_) InitializeSDL();
    return addr % 4 == 3 ? 0x00 : vram_[addr];
}

void VGA::WriteByte(std::uint32_t addr, std::uint8_t value) {
    if (!window_) InitializeSDL();
    vram_[addr] = addr % 4 == 3 ? 0xff : value;
    // update screen
    std::uint8_t *pixels;
    int pitch;
    SDL_LockTexture(screen_, nullptr,
            reinterpret_cast<void **>(&pixels), &pitch);
    pixels[addr] = vram_[addr];
    SDL_UnlockTexture(screen_);
}

std::uint16_t VGA::ReadHalf(std::uint32_t addr) {
    assert((addr & 1) == 0);
    if (!window_) InitializeSDL();
    std::uint16_t half = addr % 4 == 2 ?
            vram_[addr] : vram_[addr] | (vram_[addr + 1] << 8);
    return half;
}

void VGA::WriteHalf(std::uint32_t addr, std::uint16_t value) {
    assert((addr & 1) == 0);
    if (!window_) InitializeSDL();
    vram_[addr] = value & 0xff;
    vram_[addr + 1] = addr % 4 == 2 ? 0xff : value >> 8;
    // update screen
    std::uint8_t *pixels;
    int pitch;
    SDL_LockTexture(screen_, nullptr,
            reinterpret_cast<void **>(&pixels), &pitch);
    pixels[addr] = vram_[addr];
    pixels[addr + 1] = vram_[addr + 1];
    SDL_UnlockTexture(screen_);
}

std::uint32_t VGA::ReadWord(std::uint32_t addr) {
    assert((addr & 3) == 0);
    if (!window_) InitializeSDL();
    std::uint32_t word = vram_[addr] | (vram_[addr + 1] << 8);
    word |= vram_[addr + 2] << 16;
    return word;
}

void VGA::WriteWord(std::uint32_t addr, std::uint32_t value) {
    assert((addr & 3) == 0);
    if (!window_) InitializeSDL();
    vram_[addr] = value & 0xff;
    vram_[addr + 1] = (value >> 8) & 0xff;
    vram_[addr + 2] = (value >> 16) & 0xff;
    vram_[addr + 3] = 0xff;
    // update screen
    std::uint8_t *pixels;
    int pitch;
    SDL_LockTexture(screen_, nullptr,
            reinterpret_cast<void **>(&pixels), &pitch);
    pixels[addr] = vram_[addr];
    pixels[addr + 1] = vram_[addr + 1];
    pixels[addr + 2] = vram_[addr + 2];
    pixels[addr + 3] = vram_[addr + 3];
    SDL_UnlockTexture(screen_);
}
