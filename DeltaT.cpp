#include "SC_PlugIn.hpp"
#include <string>

static InterfaceTable *ft;

struct DeltaT : public SCUnit
{
  enum State
  {
    kWaiting,
    kHit0,
    kHit1,
    kReleasing
  };

  DeltaT()
    : mState(kWaiting)
    , mCount(0)
    , mCmdName("/delta_t")
    , mLastDelta(0)
  {
    set_calc_function<DeltaT, &DeltaT::next>();
  }

private:

  void next(int inNumSamples) {

    mThreshold = in0(2);
    mRelease = in0(3) * sampleRate();
    mReplyID = in0(4);

	  for (int i = 0; i < inNumSamples; ++i)
	  {
		  const float inSample0 = fabs(in(0)[i]);
		  const float inSample1 = fabs(in(1)[i]);
		  out(0)[i] = 0.f;
		  out(1)[i] = mLastDelta;

  		switch (mState)
      {
	  		case kWaiting:
		  		if (inSample0 >= mThreshold && inSample1 >= mThreshold)
          {
            hitDetected(0, i);
				  }
				  else if (inSample0 >= mThreshold)
          {
					  mState = kHit0;
				  }
  				else if (inSample1 >= mThreshold)
          {
  					mState = kHit1;
	  			};
				  break;

        case kHit0:
          ++mCount;
          if (inSample1 >= mThreshold)
          {
            hitDetected(mCount * sampleDur(), i);
          }
          else if (mCount > mRelease)
          {
            mState = kWaiting;
            mCount = 0;
          }
          break;

        case kHit1:
          ++mCount;
		  		if (inSample0 >= mThreshold)
          {
            hitDetected(mCount * sampleDur(), i);
		  		}
          else if (mCount > mRelease)
          {
            mState = kWaiting;
            mCount = 0;
          }
		  		break;

		  	case kReleasing:
		  		if (++mCount > mRelease)
          {
		  			mState = kWaiting;
		  			mCount = 0;
		  		}
		  		break;
      }
		}
  }

  void hitDetected(const float time, const int i)
  {
    mLastDelta = mCount / mRelease;
    out(0)[i] = 1.f;
    out(1)[i] = mLastDelta;
    mState = kReleasing;
    mCount = 0;
    SendNodeReply(&mParent->mNode, mReplyID, mCmdName.c_str(), 1, &time);
  }

	float mThreshold;
	float mRelease;
	State mState;
	int mCount;
	int mReplyID;
  std::string mCmdName;
  float mLastDelta;
};

PluginLoad(DeltaT) {
  ft = inTable;
  registerUnit<DeltaT>(ft, "DeltaT");
}

