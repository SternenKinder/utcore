/*
 * Ubitrack - Library for Ubiquitous Tracking
 * Copyright 2006, Technische Universitaet Muenchen, and individual
 * contributors as indicated by the @authors tag. See the 
 * copyright.txt in the distribution for a full listing of individual
 * contributors.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */


/**
 * @ingroup calibration
 * @file
 * Online rotation-only hand-eye-calibration
 *
 * @author Daniel Pustka <daniel.pustka@in.tum.de>
 */
 
#ifndef __UBITRACK_CALIBRATION_ONLINEROTHEC_H_INCLUDED__
#define __UBITRACK_CALIBRATION_ONLINEROTHEC_H_INCLUDED__


#ifdef HAVE_LAPACK

#include <utMath/ErrorVector.h>
#include <utMath/Quaternion.h>
#include <utCore.h>

namespace Ubitrack { namespace Algorithm {

/**
 * A variant of the Tsai-Lenz algorithm to compute hand-eye calibration online (rotation only).
 *
 * Given many pairs of quaternions a and b, describing relative orientations between frames, 
 * the class computes a quaternion x, s.t. ax = xb
 *
 * TODO: add error estimates to inputs and outputs!
 */
class UBITRACK_EXPORT OnlineRotHec
{
public:
	/** constructor */
	OnlineRotHec();
	
	/**
	 * a and b are the relative motion between two frames
	 */
	void addMeasurement( const Math::Quaternion& a, const Math::Quaternion& b );

	/** returns the currently estimated transformation x */
	Math::Quaternion computeResult() const;

protected:
	Math::ErrorVector< double, 3 > m_state;
};

} } // namespace Ubitrack::Algorithm

#endif // HAVE_LAPACK
#endif
