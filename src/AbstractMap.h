#ifndef ABTRACT_MAP_H
#define ABTRACT_MAP_H
/*
 * Map.h
 *
 * (c) 2018 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 #include "PqCore.h"

namespace pq
{

    class AbstractMap : public Unit
    {
        protected:
        virtual float _read () = 0;

        virtual float _write (float value) = 0;


        public:
        AbstractMap() {

        }

        virtual float read (float percent) = 0;

         float get () override {
            return _read();
        }

         float put (float value) override {
            return _write(value);
        }

    };

};
#endif