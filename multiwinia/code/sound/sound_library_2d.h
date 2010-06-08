#ifndef INCLUDED_SOUND_LIBRARY_2D
#define INCLUDED_SOUND_LIBRARY_2D


//*****************************************************************************
// Class StereoSample
//*****************************************************************************

class StereoSample
{
public:
	signed short m_left;
	signed short m_right;

	StereoSample(): m_left(0), m_right(0) {}
};


//*****************************************************************************
// Class SoundLibrary2d
//*****************************************************************************

class SoundLibrary2d
{
public:
	unsigned int	m_freq;
	unsigned int	m_samplesPerBuffer;
	
	SoundLibrary2d();
	virtual ~SoundLibrary2d();

    virtual void		SetCallback(void (*_callback) (StereoSample *, unsigned int)) = 0;
	virtual void		TopupBuffer() = 0;
	virtual void		Stop() = 0;

	virtual void		StartRecordToFile(char const *_filename) = 0;
	virtual void		EndRecordToFile() = 0;
};

class SoundLibrary2dNoSound : public SoundLibrary2d
{
public:
	void		SetCallback(void (*_callback) (StereoSample *, unsigned int));
	void		StartRecordToFile(char const *_filename);
	void		EndRecordToFile();
	void		TopupBuffer();
	void		Stop();
};

extern SoundLibrary2d *g_soundLibrary2d;


#endif
