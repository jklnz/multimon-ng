/*
 *      demod_tone.c -- Tone detector
 *
 *      Copyright (C) 2020  
 *          Valentin Saugnier (valentin.s.10@gmail.com)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ---------------------------------------------------------------------- */

#include "multimon.h"
#include "filter.h"
#include <math.h>
#include <string.h>

unsigned int tone_freq;

/* ---------------------------------------------------------------------- */

#define SAMPLE_RATE 22050
#define THRESHOLD 0.1

/* ---------------------------------------------------------------------- */
	
static void tone_init(struct demod_state *s)
{
	memset(&s->l1.tone, 0, sizeof(s->l1.tone));
}

/* ---------------------------------------------------------------------- */

static inline double process_block(const float* buffer, int length)
{
    int K;
    double coefficient;
    double W;
    double sine;
    double cosine;
    double Q0, Q1, Q2;
    double real;
    double imag;
    double magnitude;
    double scalingFactor;

    K = (int) (0.5 + (((float) length * (float) tone_freq) / SAMPLE_RATE));
    W = (2.0 * M_PI * K) / length;
    cosine = cos(W);
    sine = sin(W);
    coefficient = 2 * cos(W);
    scalingFactor = length / 2.0;

    Q0 = 0;
    Q1 = 0;
    Q2 = 0;

    for (; length > 0; length--, buffer++) {
        Q0 = *buffer + (coefficient * Q1) - Q2;
        Q2 = Q1;
        Q1 = Q0;
    }
    real = (Q0 - (Q1 * cosine)) / scalingFactor;
    imag = (- Q1 * sine) / scalingFactor;
    magnitude = sqrt(real * real + imag * imag);
    verbprintf(10, "%lf\n", magnitude);
    return magnitude;
}

/* ---------------------------------------------------------------------- */

static void tone_demod(struct demod_state *s, buffer_t buffer, int length)
{
    if (process_block(buffer.fbuffer, length) > THRESHOLD) {
        verbprintf(0, "TONE: %d\n", tone_freq);
    }
}
				
/* ---------------------------------------------------------------------- */

const struct demod_param demod_tone = {
    "TONE", true, SAMPLE_RATE, 0, tone_init, tone_demod, NULL
};

/* ---------------------------------------------------------------------- */
