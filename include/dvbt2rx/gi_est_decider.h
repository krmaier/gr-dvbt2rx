/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_DVBT2RX_GI_EST_DECIDER_H
#define INCLUDED_DVBT2RX_GI_EST_DECIDER_H

#include <dvbt2rx/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace dvbt2rx {

    /*!
     * \brief <+description of block+>
     * \ingroup dvbt2rx
     *
     */
    class DVBT2RX_API gi_est_decider : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<gi_est_decider> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dvbt2rx::gi_est_decider.
       *
       * To avoid accidental use of raw pointers, dvbt2rx::gi_est_decider's
       * constructor is in a private implementation
       * class. dvbt2rx::gi_est_decider::make is the public interface for
       * creating new instances.
       */
      static sptr make(float thresh_factor, int avg_syms);
    };

  } // namespace dvbt2rx
} // namespace gr

#endif /* INCLUDED_DVBT2RX_GI_EST_DECIDER_H */

