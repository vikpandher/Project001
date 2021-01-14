#pragma once

#include <stack>
#include <vector>



namespace Project001
{
	template <typename T>
	class DenseVector
	{
	public:
		DenseVector();
		~DenseVector();

		DenseVector(DenseVector& other) = delete;
		void operator=(const DenseVector&) = delete;

		T& operator[](std::size_t index);
		const T& operator[](std::size_t index) const;

	protected:

	private:

		std::vector<bool> filledFlags_;
		std::stack<unsigned int> emptyIndexStack_;
		std::vector<T> dataContainer_;
	};

	template <typename T>
	inline T& DenseVector<T>::operator[](std::size_t index)
	{
		return dataContainer_[index];
	}

	template <typename T>
	inline const T& DenseVector<T>::operator[](std::size_t index) const
	{
		return dataContainer_[index];
	}
}