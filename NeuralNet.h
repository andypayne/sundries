#ifndef _NEURALNET_H
#define _NEURALNET_H

// Simple McCulloch-Pitts Neurode
//
class MPNeurode
{
public:
	MPNeurode () :
		m_Threshold(0.0),
		m_In1(0.0),
		m_In2(0.0),
		m_Weight1(0.0),
		m_Weight2(0.0)
	{
	}


	virtual ~MPNeurode ()
	{
	}


	void SetThreshold (const float Threshold) { m_Threshold = Threshold; }
	float GetThreshold () const { return m_Threshold; }
	void SetIn1 (const float In1) { m_In1 = In1; }
	float GetIn1 () const { return m_In1; }
	void SetIn2 (const float In2) { m_In2 = In2; }
	float GetIn2 () const { return m_In2; }
	void SetWeight1 (const float Weight1) { m_Weight1 = Weight1; }
	float GetWeight1 () const { return m_Weight1; }
	void SetWeight2 (const float Weight2) { m_Weight2 = Weight2; }
	float GetWeight2 () const { return m_Weight2; }

	float Compute ()
	{
		float ActivationFn = m_In1*m_Weight1 + m_In2*m_Weight2;
		if (ActivationFn >= m_Threshold)
		{
			return 1.0;
		}
		return 0.0;
	}

	void Print ()
	{
		printf("Threshold:   %f\n", GetThreshold());
		printf("Input1:      %f\n", GetIn1());
		printf("Input2:      %f\n", GetIn2());
		printf("Weight1:     %f\n", GetWeight1());
		printf("Weight2:     %f\n", GetWeight2());
		printf("Output:      %f\n", Compute());
	}


protected:
	// Theta -- threshold
	float	m_Threshold;

	// Inputs
	float	m_In1;
	float	m_In2;

	// Weights on the two inputs
	float	m_Weight1;
	float	m_Weight2;

};




#ifdef NEURALNET_UNITTEST

class UnitTestNeuralNet
{
public:
	int Test ()
	{
		MPNeurode	mpn;

		//
		// AND Gate
		//

		printf("\nAND Gate:\n");
		mpn.SetThreshold(2.0);
		mpn.SetWeight1(1.0);
		mpn.SetWeight2(1.0);

		{
			mpn.SetIn1(0.0);
			mpn.SetIn2(0.0);
			printf("\n");
			mpn.Print();
		}


		{
			mpn.SetIn1(0.0);
			mpn.SetIn2(1.0);
			printf("\n");
			mpn.Print();
		}


		{
			mpn.SetIn1(1.0);
			mpn.SetIn2(0.0);
			printf("\n");
			mpn.Print();
		}


		{
			mpn.SetIn1(1.0);
			mpn.SetIn2(1.0);
			printf("\n");
			mpn.Print();
		}

    return 0;
	}
};

#endif


#endif	// #ifndef NEURALNET_H

