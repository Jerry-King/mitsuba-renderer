/*
    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2010 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined(__CSTREAM_H)
#define __CSTREAM_H

#include <mitsuba/mitsuba.h>

MTS_NAMESPACE_BEGIN

/** \brief Console stream
 */
class MTS_EXPORT_CORE ConsoleStream : public Stream {
public:
	/// Create a new console stream
	ConsoleStream();

	/* Stream implementation */
	void read(void *ptr, size_t size);
	void write(const void *ptr, size_t size);
	void setPos(size_t pos);
	size_t getPos() const;
	size_t getSize() const;
	void truncate(size_t size);
	void flush();
	bool canWrite() const;
	bool canRead() const;

	/// Return a string representation
	std::string toString() const;

	MTS_DECLARE_CLASS()
protected:
	/** \brief Virtual destructor
	 *
	 * The destructor frees all resources and closes
	 * the socket if it is still open
	 */
	virtual ~ConsoleStream();
};

MTS_NAMESPACE_END

#endif /* __CSTREAM_H */
