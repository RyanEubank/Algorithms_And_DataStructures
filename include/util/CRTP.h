/* ============================================================================
* Copyright (C) 2023 Ryan Eubank
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
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
* ========================================================================= */

#pragma once

namespace collections {

	template <class derived_t, class base_t>
	class CRTP {
	protected:

		[[nodiscard]] derived_t& self() {
			return static_cast<derived_t&>(*this);
		}

		[[nodiscard]] const derived_t& self() const {
			return static_cast<const derived_t&>(*this);
		}

	private:

		CRTP() {}

		friend base_t;
	};
}