
#include <utMath/Pose.h>
#include <utMath/Vector.h>
#include <utMath/Matrix.h>
#include <utCalibration/HandEyeCalibrationDual.h>

#include <utMath/Random/Scalar.h>
#include <utMath/Random/Vector.h>
#include <utMath/Random/Rotation.h>
#include "../tools.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>


using namespace Ubitrack::Math;

#ifndef HAVE_LAPACK
void TestHandEye()
{
	// HandyEye does not work without lapack
}
#else // HAVE_LAPACK

template< typename T >
void testDualHandEyeMatrixRandom( const std::size_t n_runs, const T epsilon )
{
	typename Random::Quaternion< T >::Uniform randQuat;
	typename Random::Vector< T, 3 >::Uniform randVector( -100, 100 );
	
	for ( std::size_t iRun = 0; iRun < n_runs; iRun++ )
	{
		const std::size_t n( Random::distribute_uniform< std::size_t >( 4, 30 ) );

		//set first target frame
		std::vector< Matrix< T, 4, 4 > > rightFrame;
		rightFrame.reserve( n );
		for( std::size_t i = 0; i<n; ++i )
		{
			Quaternion q1 = randQuat();
			Vector< T, 3 > t1 = randVector();	
			Matrix< T, 4, 4 > mat1( q1, t1 );
			rightFrame.push_back( mat1 );
		}
		
		// produce other target frame
		Quaternion q = randQuat();
		Vector< T, 3 > t = randVector();
		const Matrix< T, 4, 4 > mat( q, t );
		
		std::vector< Matrix< T, 4, 4 > > leftFrame;
		leftFrame.reserve( n );
		for( std::size_t i = 0; i<n; ++i )
		{
			leftFrame.push_back( boost::numeric::ublas::prod( mat, rightFrame[ i ] ) );
		}

		// do some estimation now
		const Pose estimatedPose;// = Ubitrack::Calibration::performHandEyeCalibration ( leftFrame, rightFrame, true );
		
		// calculate some errors
		const T rotDiff = quaternionDiff( estimatedPose.rotation(), q );
		const T posDiff = vectorDiff( estimatedPose.translation(), t );
		// if( b_done )
		{
			// check if pose is better than before (only for valid results)
			BOOST_CHECK_MESSAGE( rotDiff < epsilon, "\nEstimated rotation from " << n << " poses resulted in error " << rotDiff << " :\n" << q << " (expected)\n" << estimatedPose.rotation() << " (estimated)\n" );
			BOOST_CHECK_MESSAGE( posDiff < epsilon, "\nEstimated position from " << n << " poses resulted in error " << posDiff << " :\n" << t << " (expected)\n" << estimatedPose.translation() << " (estimated\n" );
		}
		// BOOST_WARN_MESSAGE( b_done, "Algorithm did not succesfully estimate a result with " << n 
			// << " points.\nRemaining difference in rotation " << rotDiff << ", difference in translation " << posDiff << "." );
	}
}

template< typename T >
void testDualHandEyePoseRandom( const std::size_t n_runs, const T epsilon )
{
	typename Random::Quaternion< T >::Uniform randQuat;
	typename Random::Vector< T, 3 >::Uniform randVector( -10., 10. );
	
	for ( std::size_t iRun = 0; iRun < n_runs; iRun++ )
	{
		const std::size_t n( Random::distribute_uniform< std::size_t >( 4, 30 ) );

		//generate a radnom pose
		Quaternion q = randQuat();
		Vector< T, 3 > t = randVector();
		const Pose pose( q, t );
		
		//set first target frame
		std::vector< Pose > rightFrame;
		rightFrame.reserve( n );
		
		//set the second target frame
		std::vector< Pose > leftFrame;
		leftFrame.reserve( n );
		
		for( std::size_t i = 0; i<n; ++i )
		{
			Quaternion q1 = randQuat();
			Vector< T, 3 > t1 = randVector();	
			Pose p1( q1, t1 );
			rightFrame.push_back( p1 );
			leftFrame.push_back( ~(pose * p1) );
		}

		// do some estimation now
		Pose estimatedPose;
		bool b_done = Ubitrack::Calibration::estimatePose6D_6D6D ( leftFrame, estimatedPose, rightFrame );
		
		if( b_done )
		{
			// calculate some errors
			const T rotDiff = quaternionDiff( estimatedPose.rotation(), q );
			const T posDiff = vectorDiff( estimatedPose.translation(), t );
			
			// check if pose is better than before (only for valid results)
			BOOST_CHECK_MESSAGE( rotDiff < epsilon, "\nEstimated rotation from " << n << " poses resulted in error " << rotDiff << " :\n" << q << " (expected)\n" << estimatedPose.rotation() << " (estimated)\n" );
			BOOST_CHECK_MESSAGE( posDiff < epsilon, "\nEstimated position from " << n << " poses resulted in error " << posDiff << " :\n" << t << " (expected)\n" << estimatedPose.translation() << " (estimated)\n" );
		}
		BOOST_WARN_MESSAGE( b_done, "Algorithm did not succesfully estimate a result from " << n << " poses." );
	}
}

void TestDualHandEye()
{
	// testDualHandEyeMatrixRandom< float >( 10, 1e-2f );
	// testDualHandEyeMatrixRandom< double >( 10, 1e-6 );
	testDualHandEyePoseRandom< double >( 100, 1e-6 );
}

#endif // HAVE_LAPACK
