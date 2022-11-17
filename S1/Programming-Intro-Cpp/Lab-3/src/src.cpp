// src.cpp : Defines the entry point for the application.
//

#include "src.h"

int main(int argc, char** argv) {
    interactions::config config(argc, argv);

    imgmanip::color_distribution color;
    sandpile simulation;

    if (!config.input.empty()) {
        simulation = config.input;
    }

    if (config.length > 0) {
        simulation.set_max_height(config.length);
    }

    if (config.width > 0) {
        simulation.set_max_width(config.width);
    }

    uint32_t idx;

    for (idx = 0; (idx < config.iterations || config.iterations == 0) && !simulation.resolved(); ++idx) {
        simulation.simulate();

        if (config.freq > 0 && idx % config.freq == 0) {
            save_with_config(simulation, config, idx);
        }
    }

    save_with_config(simulation, config, idx);

    return 0;
}
