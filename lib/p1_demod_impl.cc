/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "p1_demod_impl.h"

namespace gr {
  namespace dvbt2rx {

    p1_demod::sptr
    p1_demod::make()
    {
      return gnuradio::get_initial_sptr
        (new p1_demod_impl());
    }

    /*
     * The private constructor
     */
    p1_demod_impl::p1_demod_impl()
      : gr::sync_block("p1_demod",
              gr::io_signature::make3(3, 3, sizeof(char),sizeof(gr_complex),sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
    	set_history(1024+542);
    }

    /*
     * Our virtual destructor.
     */
    p1_demod_impl::~p1_demod_impl()
    {
    }

    int
    p1_demod_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
    	const char *in_sync = (const char *) input_items[0];
    	const gr_complex *in_phase = (const gr_complex *) input_items[1];
    	const gr_complex *in_data = (const gr_complex *) input_items[2];
    	gr_complex *out = (gr_complex *) output_items[0];

    	memcpy(out, in_data, noutput_items*sizeof(gr_complex));

    	for(int i=0; i<noutput_items; i++){
    		if(in_sync[i]){
    			float ffo = std::arg(in_phase[i]);
    			//s=demod(in_data+i, ffo)
    			add_item_tag(0,nitems_written(0)+i, pmt::mp("p1_start"), pmt::mp("changeme"));

    		}
    	}

      return noutput_items;
    }



    const int p1_demod_impl::p1_active_carriers[384] =
    {
        44, 45, 47, 51, 54, 59, 62, 64, 65, 66, 70, 75, 78, 80, 81, 82, 84, 85, 87, 88, 89, 90,
        94, 96, 97, 98, 102, 107, 110, 112, 113, 114, 116, 117, 119, 120, 121, 122, 124,
        125, 127, 131, 132, 133, 135, 136, 137, 138, 142, 144, 145, 146, 148, 149, 151,
        152, 153, 154, 158, 160, 161, 162, 166, 171,

        172, 173, 175, 179, 182, 187, 190, 192, 193, 194, 198, 203, 206, 208, 209, 210,
        212, 213, 215, 216, 217, 218, 222, 224, 225, 226, 230, 235, 238, 240, 241, 242,
        244, 245, 247, 248, 249, 250, 252, 253, 255, 259, 260, 261, 263, 264, 265, 266,
        270, 272, 273, 274, 276, 277, 279, 280, 281, 282, 286, 288, 289, 290, 294, 299,
        300, 301, 303, 307, 310, 315, 318, 320, 321, 322, 326, 331, 334, 336, 337, 338,
        340, 341, 343, 344, 345, 346, 350, 352, 353, 354, 358, 363, 364, 365, 367, 371,
        374, 379, 382, 384, 385, 386, 390, 395, 396, 397, 399, 403, 406, 411, 412, 413,
        415, 419, 420, 421, 423, 424, 425, 426, 428, 429, 431, 435, 438, 443, 446, 448,
        449, 450, 454, 459, 462, 464, 465, 466, 468, 469, 471, 472, 473, 474, 478, 480,
        481, 482, 486, 491, 494, 496, 497, 498, 500, 501, 503, 504, 505, 506, 508, 509,
        511, 515, 516, 517, 519, 520, 521, 522, 526, 528, 529, 530, 532, 533, 535, 536,
        537, 538, 542, 544, 545, 546, 550, 555, 558, 560, 561, 562, 564, 565, 567, 568,
        569, 570, 572, 573, 575, 579, 580, 581, 583, 584, 585, 586, 588, 589, 591, 595,
        598, 603, 604, 605, 607, 611, 612, 613, 615, 616, 617, 618, 622, 624, 625, 626,
        628, 629, 631, 632, 633, 634, 636, 637, 639, 643, 644, 645, 647, 648, 649, 650,
        654, 656, 657, 658, 660, 661, 663, 664, 665, 666, 670, 672, 673, 674, 678, 683,

        684, 689, 692, 696, 698, 699, 701, 702, 703, 704, 706, 707, 708,
        712, 714, 715, 717, 718, 719, 720, 722, 723, 725, 726, 727, 729,
        733, 734, 735, 736, 738, 739, 740, 744, 746, 747, 748, 753, 756,
        760, 762, 763, 765, 766, 767, 768, 770, 771, 772, 776, 778, 779,
        780, 785, 788, 792, 794, 795, 796, 801, 805, 806, 807, 809
    };

    const unsigned char p1_demod_impl::s1_modulation_patterns[8][8] =
    {
        {0x12, 0x47, 0x21, 0x74, 0x1D, 0x48, 0x2E, 0x7B},
        {0x47, 0x12, 0x74, 0x21, 0x48, 0x1D, 0x7B, 0x2E},
        {0x21, 0x74, 0x12, 0x47, 0x2E, 0x7B, 0x1D, 0x48},
        {0x74, 0x21, 0x47, 0x12, 0x7B, 0x2E, 0x48, 0x1D},
        {0x1D, 0x48, 0x2E, 0x7B, 0x12, 0x47, 0x21, 0x74},
        {0x48, 0x1D, 0x7B, 0x2E, 0x47, 0x12, 0x74, 0x21},
        {0x2E, 0x7B, 0x1D, 0x48, 0x21, 0x74, 0x12, 0x47},
        {0x7B, 0x2E, 0x48, 0x1D, 0x74, 0x21, 0x47, 0x12}
    };

    const unsigned char p1_demod_impl::s2_modulation_patterns[16][32] =
    {
        {0x12, 0x1D, 0x47, 0x48, 0x21, 0x2E, 0x74, 0x7B, 0x1D, 0x12, 0x48, 0x47, 0x2E, 0x21, 0x7B, 0x74,
         0x12, 0xE2, 0x47, 0xB7, 0x21, 0xD1, 0x74, 0x84, 0x1D, 0xED, 0x48, 0xB8, 0x2E, 0xDE, 0x7B, 0x8B},
        {0x47, 0x48, 0x12, 0x1D, 0x74, 0x7B, 0x21, 0x2E, 0x48, 0x47, 0x1D, 0x12, 0x7B, 0x74, 0x2E, 0x21,
         0x47, 0xB7, 0x12, 0xE2, 0x74, 0x84, 0x21, 0xD1, 0x48, 0xB8, 0x1D, 0xED, 0x7B, 0x8B, 0x2E, 0xDE},
        {0x21, 0x2E, 0x74, 0x7B, 0x12, 0x1D, 0x47, 0x48, 0x2E, 0x21, 0x7B, 0x74, 0x1D, 0x12, 0x48, 0x47,
         0x21, 0xD1, 0x74, 0x84, 0x12, 0xE2, 0x47, 0xB7, 0x2E, 0xDE, 0x7B, 0x8B, 0x1D, 0xED, 0x48, 0xB8},
        {0x74, 0x7B, 0x21, 0x2E, 0x47, 0x48, 0x12, 0x1D, 0x7B, 0x74, 0x2E, 0x21, 0x48, 0x47, 0x1D, 0x12,
         0x74, 0x84, 0x21, 0xD1, 0x47, 0xB7, 0x12, 0xE2, 0x7B, 0x8B, 0x2E, 0xDE, 0x48, 0xB8, 0x1D, 0xED},
        {0x1D, 0x12, 0x48, 0x47, 0x2E, 0x21, 0x7B, 0x74, 0x12, 0x1D, 0x47, 0x48, 0x21, 0x2E, 0x74, 0x7B,
         0x1D, 0xED, 0x48, 0xB8, 0x2E, 0xDE, 0x7B, 0x8B, 0x12, 0xE2, 0x47, 0xB7, 0x21, 0xD1, 0x74, 0x84},
        {0x48, 0x47, 0x1D, 0x12, 0x7B, 0x74, 0x2E, 0x21, 0x47, 0x48, 0x12, 0x1D, 0x74, 0x7B, 0x21, 0x2E,
         0x48, 0xB8, 0x1D, 0xED, 0x7B, 0x8B, 0x2E, 0xDE, 0x47, 0xB7, 0x12, 0xE2, 0x74, 0x84, 0x21, 0xD1},
        {0x2E, 0x21, 0x7B, 0x74, 0x1D, 0x12, 0x48, 0x47, 0x21, 0x2E, 0x74, 0x7B, 0x12, 0x1D, 0x47, 0x48,
         0x2E, 0xDE, 0x7B, 0x8B, 0x1D, 0xED, 0x48, 0xB8, 0x21, 0xD1, 0x74, 0x84, 0x12, 0xE2, 0x47, 0xB7},
        {0x7B, 0x74, 0x2E, 0x21, 0x48, 0x47, 0x1D, 0x12, 0x74, 0x7B, 0x21, 0x2E, 0x47, 0x48, 0x12, 0x1D,
         0x7B, 0x8B, 0x2E, 0xDE, 0x48, 0xB8, 0x1D, 0xED, 0x74, 0x84, 0x21, 0xD1, 0x47, 0xB7, 0x12, 0xE2},
        {0x12, 0xE2, 0x47, 0xB7, 0x21, 0xD1, 0x74, 0x84, 0x1D, 0xED, 0x48, 0xB8, 0x2E, 0xDE, 0x7B, 0x8B,
         0x12, 0x1D, 0x47, 0x48, 0x21, 0x2E, 0x74, 0x7B, 0x1D, 0x12, 0x48, 0x47, 0x2E, 0x21, 0x7B, 0x74},
        {0x47, 0xB7, 0x12, 0xE2, 0x74, 0x84, 0x21, 0xD1, 0x48, 0xB8, 0x1D, 0xED, 0x7B, 0x8B, 0x2E, 0xDE,
         0x47, 0x48, 0x12, 0x1D, 0x74, 0x7B, 0x21, 0x2E, 0x48, 0x47, 0x1D, 0x12, 0x7B, 0x74, 0x2E, 0x21},
        {0x21, 0xD1, 0x74, 0x84, 0x12, 0xE2, 0x47, 0xB7, 0x2E, 0xDE, 0x7B, 0x8B, 0x1D, 0xED, 0x48, 0xB8,
         0x21, 0x2E, 0x74, 0x7B, 0x12, 0x1D, 0x47, 0x48, 0x2E, 0x21, 0x7B, 0x74, 0x1D, 0x12, 0x48, 0x47},
        {0x74, 0x84, 0x21, 0xD1, 0x47, 0xB7, 0x12, 0xE2, 0x7B, 0x8B, 0x2E, 0xDE, 0x48, 0xB8, 0x1D, 0xED,
         0x74, 0x7B, 0x21, 0x2E, 0x47, 0x48, 0x12, 0x1D, 0x7B, 0x74, 0x2E, 0x21, 0x48, 0x47, 0x1D, 0x12},
        {0x1D, 0xED, 0x48, 0xB8, 0x2E, 0xDE, 0x7B, 0x8B, 0x12, 0xE2, 0x47, 0xB7, 0x21, 0xD1, 0x74, 0x84,
         0x1D, 0x12, 0x48, 0x47, 0x2E, 0x21, 0x7B, 0x74, 0x12, 0x1D, 0x47, 0x48, 0x21, 0x2E, 0x74, 0x7B},
        {0x48, 0xB8, 0x1D, 0xED, 0x7B, 0x8B, 0x2E, 0xDE, 0x47, 0xB7, 0x12, 0xE2, 0x74, 0x84, 0x21, 0xD1,
         0x48, 0x47, 0x1D, 0x12, 0x7B, 0x74, 0x2E, 0x21, 0x47, 0x48, 0x12, 0x1D, 0x74, 0x7B, 0x21, 0x2E},
        {0x2E, 0xDE, 0x7B, 0x8B, 0x1D, 0xED, 0x48, 0xB8, 0x21, 0xD1, 0x74, 0x84, 0x12, 0xE2, 0x47, 0xB7,
         0x2E, 0x21, 0x7B, 0x74, 0x1D, 0x12, 0x48, 0x47, 0x21, 0x2E, 0x74, 0x7B, 0x12, 0x1D, 0x47, 0x48},
        {0x7B, 0x8B, 0x2E, 0xDE, 0x48, 0xB8, 0x1D, 0xED, 0x74, 0x84, 0x21, 0xD1, 0x47, 0xB7, 0x12, 0xE2,
         0x7B, 0x74, 0x2E, 0x21, 0x48, 0x47, 0x1D, 0x12, 0x74, 0x7B, 0x21, 0x2E, 0x47, 0x48, 0x12, 0x1D}
    };

  } /* namespace dvbt2rx */
} /* namespace gr */

