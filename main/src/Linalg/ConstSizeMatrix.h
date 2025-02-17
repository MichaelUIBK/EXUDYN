/** ***********************************************************************************************
* @class		ConstSizeMatrix
* @brief		A matrix derived from Matrix for math operations with templated maximum size and memory allocated on local heap 
* @details		Details:
                    - a matrix of Real entries (Real/float);
                    - use SlimMatrix for tiny matrices with known size
                    - use LinkedDataMatrix to link data to a (part of a) matrix (without memory allocation)
                    - use ResizableMatrix to allow a matrix to allocate more data than currently needed (no memory allocation when matrix size changes)
*
* @author		Gerstmayr Johannes
* @date			1997-05-15 (generated)
* @date			2019-05-13 (last modified)
* @pre			Indizes of []-operator run from 0 to dataSize-1;
* @copyright	This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: https://github.com/jgerstmayr/EXUDYN
* 				
*
* *** Example code ***
*
* @code{.cpp}
* ConstSizeMatrix<50> m(5, 5);			//create a matrix with 25 Real entries, allocated 50 entries
* for (Index i = 0; i < 5; i++) {
*   for (Index j = 0; j < 5; j++) {
*     m(i,j) = i*j;
* }
* ConstSizeMatrix<25> m2 = m1;	//assign m1 to m2; m2 has different max size than m1
* m1 += m2;					    //add m2 to m1
* cout << v1 << "\n";			//write m1 to cout
* @endcode
************************************************************************************************ */
#ifndef CONSTSIZEMATRIX__H
#define CONSTSIZEMATRIX__H

#include <initializer_list> //for initializer_list in constructor
#include <ostream>          //ostream for matrix output as text
#include <iostream>         //for cout @todo remove cout from class matrix ==> add Error handling

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"
#include "Linalg/Matrix.h"

#ifdef USE_NEW_CONSTSIZEMATRIX

template <typename T, Index dataSize>
class ConstSizeMatrixBase
{
private:
	mutable T data[dataSize];
	Index numberOfRows;
	Index numberOfColumns;

public:

	//Constructors, Destructor

	//! no default constructor, rule of 5
	//ConstSizeMatrixBase(bool init = true)
		
	//! create empty (0 x 0) matrix, replaces default constructor, rule of 5
	ConstSizeMatrixBase(bool init = true)
	{
		this->numberOfRows = 0;
		this->numberOfColumns = 0;
	}

	//! copy constructor, based on iterators, rule of 5
	//ConstSizeMatrixBase(const ConstSizeMatrixBase& matrix)
	//{
	//	ResizeMatrix(matrix.NumberOfRows(), matrix.NumberOfColumns());

	//	Index cnt = 0;
	//	for (auto value : matrix) { this->data[cnt++] = value; }
	//}

	//! move constructor, rule of 5
	//ConstSizeMatrixBase(ConstSizeMatrixBase&& other) noexcept
	//{
	//	//exchange of pointers will not work! 
	//	//this->data = std::exchange(other.data, nullptr);
	//	//this->numberOfRows = std::exchange(other.numberOfRows, 0);
	//	//this->numberOfColumns = std::exchange(other.numberOfColumns, 0);
	//	for (Index i = 0; i < other.numberOfRows*other.numberOfColumns; i++) {
	//		constData[i] = std::exchange(other.constData[i], (T)0);
	//	}
	//	this->numberOfRows = std::exchange(other.numberOfRows, 0);
	//	this->numberOfColumns = std::exchange(other.numberOfColumns, 0);
	//}

	//! create matrix with dimensions numberOfRowsInit x numberOfColumnsInit; data is not initialized
	ConstSizeMatrixBase(Index numberOfRowsInit, Index numberOfColumnsInit)
	{
		CHECKandTHROW((numberOfRowsInit >= 0 && numberOfColumnsInit >= 0 && numberOfRowsInit*numberOfColumnsInit <= dataSize),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(Index, Index): invalid parameters");

		ResizeMatrix(numberOfRowsInit, numberOfColumnsInit);
	}

	//! create matrix with dimensions numberOfRowsInit x numberOfColumnsInit; initialize items with 'initializationValue'
	ConstSizeMatrixBase(Index numberOfRowsInit, Index numberOfColumnsInit, T initializationValue)
	{
		CHECKandTHROW((numberOfRowsInit >= 0 && numberOfColumnsInit >= 0 && numberOfRowsInit*numberOfColumnsInit <= dataSize),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(Index, Index, T): invalid parameters");
		CHECKandTHROW((initializationValue == 0), "ConstSizeMatrixBase: initializationValue != 0");

		ResizeMatrix(numberOfRowsInit, numberOfColumnsInit);

		for (auto &item : *this) {
			item = initializationValue;
		}
	}

	//! create matrix with dimensions numberOfRowsInit x numberOfColumnsInit; initialize data with initializer list
	ConstSizeMatrixBase(Index numberOfRowsInit, Index numberOfColumnsInit, std::initializer_list<T> listOfReals)
	{
		CHECKandTHROW((numberOfRowsInit*numberOfColumnsInit == (Index)listOfReals.size()) && numberOfRowsInit*numberOfColumnsInit <= dataSize,
			"ConstSizeMatrixBase::ConstSizeMatrixBase(Index, Index, initializer_list): inconsistent size of initializer_list");

		ResizeMatrix(numberOfRowsInit, numberOfColumnsInit);

		Index cnt = 0;
		for (auto value : listOfReals) {
			this->data[cnt++] = value;
		}
	}

	////! copy constructor with MatrixBase; @todo: check if ConstSizeMatrixBase(const MatrixBase& matrix) is NEEDED?
	//ConstSizeMatrixBase(const MatrixBase<T>& matrix)
	//{
	//	CHECKandTHROW((matrix.NumberOfRows()*matrix.NumberOfColumns() <= dataSize),
	//		"ConstSizeMatrixBase::ConstSizeMatrixBase(const MatrixBase& matrix): dataSize too small");

	//	ResizeMatrix(matrix.NumberOfRows(), matrix.NumberOfColumns());

	//	Index cnt = 0;
	//	for (auto value : matrix) { this->data[cnt++] = value; }
	//}

	//! copy from std::array<std::array<Real,nCols>,nRows> matrix; gcc-7 having problems with that!
	template<Index matrixColumns, Index matrixRows>
	ConstSizeMatrixBase(const std::array<std::array<T, matrixColumns>, matrixRows>& matrix)
	{
		CHECKandTHROW((matrixRows*matrixColumns <= dataSize),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(const std::array<std::array<T, matrixColumns>, matrixRows>& matrix): dataSize too small");

		ResizeMatrix(matrixRows, matrixColumns);

		Index cnt = 0;
		for (Index rows = 0; rows < matrixRows; rows++)
		{
			for (Index cols = 0; cols < matrixColumns; cols++)
			{
				data[cnt++] = matrix[rows][cols];
			}
		}
	}

	ConstSizeMatrixBase(const StdArray33F& matrix)
	{
		CHECKandTHROW((dataSize == 9),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(const StdArray33F& matrix& matrix): dataSize invalid");

		ResizeMatrix(3, 3);

		Index cnt = 0;
		for (Index rows = 0; rows < 3; rows++)
		{
			for (Index cols = 0; cols < 3; cols++)
			{
				data[cnt++] = matrix[rows][cols];
			}
		}
	}

	//! destructor, rule of 5
	//~ConstSizeMatrixBase<T, dataSize>()
	//{
	//};


	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// BASIC FUNCTIONS
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	T* begin() const { return &data[0]; }						            //!< C++11 std::begin() for iterators; iterator range is always the currently used numberOfItems.
	//const T* begin() const { return &data[0]; }				                //!< C++11 std::begin() for iterators, const version needed for ==, +=, etc.; iterator range is always the currently used numberOfItems.
	T* end() const { return &data[numberOfRows*numberOfColumns]; }			//!< C++11 std::end() for iterators; iterator range is always the currently used numberOfItems.
	//const T* end() const { return &data[numberOfRows*numberOfColumns]; }	//!< C++11 std::end() for iterators, const version needed for ==, +=, etc.; iterator range is always the currently used numberOfItems.

	Index NumberOfRows() const { return numberOfRows; };                            //!< number of columns (currently used)
	Index NumberOfColumns() const { return numberOfColumns; };                      //!< number of rows (currently used)
	T* GetDataPointer() const { return data; }									//!< return pointer to first data containing T numbers; const needed for LinkedDataVectors.
	bool IsConstSizeMatrix() const { return true; }						//!< for derived classes: determine, if matrix has constant size

	//! Set rows and columns sizes (also used in derived classes)
	void SetNumberOfRowsAndColumns(Index numberOfRowsInit, Index numberOfColumnsInit)
	{
		CHECKandTHROW((numberOfRowsInit*numberOfColumnsInit <= dataSize), "ConstSizeMatrixBase::SetNumberOfRowsAndColumns: numberOfRowsInit*numberOfColumnsInit > dataSize");
		ResizeMatrix(numberOfRowsInit, numberOfColumnsInit); //do not check if size unchanged, is faster to set values
	}

	//! copy a submatrix from matrix, ranging rows[matrixStartRow,matrixEndRow] and columns[matrixEndColumns]; CopyFrom(m, 0,1,0,1) copy the item m(0,0)
	template<class TMatrix>
	void CopyFrom(const TMatrix& matrix, Index matrixStartRow, Index matrixStartColumn, Index matrixEndRow, Index matrixEndColumn)
	{
		//omit checks, as there would be really many checks necessary
#ifndef EXUDYN_RELEASE
		CHECKandTHROW((matrixEndRow >= matrixStartRow && matrixEndColumn >= matrixStartColumn) && 
			((matrixEndRow - matrixStartRow)*(matrixEndColumn - matrixStartColumn) <= dataSize) &&
			(matrixEndRow <= matrix.NumberOfRows()) && (matrixEndColumn <= matrix.NumberOfColumns()),
			"ConstSizeMatrixBase::CopyFrom(const MatrixBase&, ...): invalid parameters");
#endif
		ResizeMatrix(matrixEndRow - matrixStartRow, matrixEndColumn - matrixStartColumn);
		Index i = 0;
		for (Index row = matrixStartRow; row < matrixEndRow; row++)
		{
			for (Index column = matrixStartColumn; column < matrixEndColumn; column++)
			{
				data[i++] = matrix(row, column);
			}
		}
	}

	//! make a real copy of matrix
	template<class TMatrix>
	void CopyFrom(const TMatrix& matrix)
	{
		CHECKandTHROW((matrix.NumberOfRows() * matrix.NumberOfColumns() <= dataSize), "ConstSizeMatrixBase::CopyFrom<TMatrix>(...): matrixRows*matrixColumns > dataSize");
		ResizeMatrix(matrix.NumberOfRows(), matrix.NumberOfColumns());

		Index cnt = 0;
		for (auto value : matrix) { data[cnt++] = (T)value; }
	}

	//! set all items to given value.
	void SetAll(T value)
	{
		for (auto &item : *this) {
			item = value;
		}
	}

	//! set matrix with dimensions numberOfRowsInit x numberOfColumnsInit; initialize data with initializer list
	void SetMatrix(Index numberOfRowsInit, Index numberOfColumnsInit, std::initializer_list<T> listOfTs)
	{
		CHECKandTHROW((numberOfRowsInit*numberOfColumnsInit == (Index)listOfTs.size()),
			"Matrix::SetMatrix(Index, Index, initializer_list): inconsistent size of initializer_list");
		ResizeMatrix(numberOfRowsInit, numberOfColumnsInit);

		Index cnt = 0;
		for (auto value : listOfTs) {
			data[cnt++] = value;
		}
	}

	//Set Matrix with components 'value' in the diagonal and zero elsewhere
	void SetScalarMatrix(Index rowsColumns, T value)
	{
		CHECKandTHROW((rowsColumns*rowsColumns <= dataSize), "ConstSizeMatrixBase::SetScalarMatrix: rowsColumns*rowsColumns > dataSize");
		SetNumberOfRowsAndColumns(rowsColumns, rowsColumns); //JG2019-05-13: changed from ResizeMatrix(...)

		for (Index i = 0; i < rowsColumns*rowsColumns; i++)
		{
			data[i] = 0;
		}
		for (Index j = 0; j < rowsColumns; j++)
		{
			GetUnsafe(j, j) = value;
		}

	}

protected:
	//! Set new size of matrix; for external access, use 'SetNumberOfRowsAndColumns' to modify size of matrix
	void ResizeMatrix(Index numberOfRowsInit, Index numberOfColumnsInit)
	{
		//checks to be done in caller functions!
		this->numberOfRows = numberOfRowsInit;
		this->numberOfColumns = numberOfColumnsInit;
	}

public:
	//Assignment-operator, copy only active data from matrix, rule of 5
	//ConstSizeMatrixBase<T, dataSize>& operator= (const ConstSizeMatrixBase<T, dataSize>& matrix)
	//{
	//	if (this == &matrix) { return *this; }
	//	ResizeMatrix(matrix.NumberOfRows(), matrix.NumberOfColumns());

	//	Index cnt = 0;
	//	for (auto value : matrix) { this->data[cnt++] = value; }
	//	return *this;
	//}

	//! move assignment operator, rule of 5 (but does not work in such a way)
	//leads immediatly to crash
	//ConstSizeMatrixBase<T, dataSize>& operator= (ConstSizeMatrixBase<T, dataSize>&& other) = default;
	//ConstSizeMatrixBase<T, dataSize>& operator= (ConstSizeMatrixBase<T, dataSize>&& other) noexcept
	//{
	//	std::swap(*this, other); //seems to work for static data?
	//	return *this;

	//	//std::swap(this->numberOfRows, other.numberOfRows);
	//	//std::swap(this->numberOfColumns, other.numberOfColumns);
	//	//return *this;
	//}
	//! const (read) access of item with index 'i'; items run in range[0, numberOfRows*numberOfColumns]
	const T& GetItem(Index index) const
	{
		//CHECKandTHROW((index >= 0) && "Matrix::GetItem(Index) const: index < 0");
		CHECKandTHROW((index < numberOfRows*numberOfColumns), "Matrix::GetItem(Index) const: index >= numberOfRows*numberOfColumns");
		return data[index];
	}

	//! by reference (write) access of item with index 'i'; does not automatically increase array (compatibility with SlimArray<>)
	T& GetItem(Index index)
	{
		CHECKandTHROW((index < numberOfRows*numberOfColumns), "Matrix::GetItem(Index): index >= numberOfRows*numberOfColumns");
		return data[index];
	}

	//! const (read) access of item with index 'i'; items run in range[0, numberOfRows*numberOfColumns]
	const T& GetItemUnsafe(Index index) const
	{
		return data[index];
	}

	//! by reference (write) access of item with index 'i'; does not automatically increase array (compatibility with SlimArray<>)
	T& GetItemUnsafe(Index index)
	{
		return data[index];
	}

	//Referencing access-operator on element using row- and column-values
	T& GetItem(Index row, Index column)
	{
		CHECKandTHROW((row < numberOfRows), "Matrix::GetItem()(Index, Index): request of invalid row");
		CHECKandTHROW((column < numberOfColumns), "Matrix::GetItem()(Index, Index): request of invalid column");

		return data[row*numberOfColumns + column];
	}

	//Referencing constant access-operator on element using row- and column-values, WARNING: ZERO-BASED (DIFFERENT TO HOTINT1)
	const T& GetItem(Index row, Index column) const
	{
		CHECKandTHROW((row < numberOfRows), "Matrix::GetItem()(Index, Index) const: request of invalid row");
		CHECKandTHROW((column < numberOfColumns), "Matrix::GetItem()(Index, Index) const: request of invalid column");

		return data[row*numberOfColumns + column];
	};

	//Referencing access-operator UNCHECKED on element using row- and column-values
	T& GetUnsafe(Index row, Index column)
	{
		return data[row*numberOfColumns + column];
	}

	//Referencing constant access-operator UNCHECKED on element using row- and column-values, WARNING: ZERO-BASED (DIFFERENT TO HOTINT1)
	const T& GetUnsafe(Index row, Index column) const
	{
		return data[row*numberOfColumns + column];
	};

	//void CopyFrom(const Matrix& m, Index row1, Index col1, Index row2, Index col2, const IVector& r);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// OPERATORS
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! get pointer to row with Index 'row'; allows matrix access with [row][column] style (column is C-based array and does not include range check/assertion!!!)
	T* operator[](Index row)
	{
		CHECKandTHROW(/*(row >= 0) && */(row < numberOfRows), "Matrix::operator[](Index): request of invalid row");

		return &(data[row*numberOfColumns]);
	}

	T* operator[](Index row) const
	{
		CHECKandTHROW(/*(row >= 0) && */(row < numberOfRows), "Matrix::operator[](Index) const: request of invalid row");

		return &(data[row*numberOfColumns]);
	}

	//Referencing access-operator on element using row- and column-values
	T& operator()(Index row, Index column)
	{
		CHECKandTHROW(/*(row >= 0) && */(row < numberOfRows), "Matrix::operator()(Index, Index): request of invalid row");
		CHECKandTHROW(/*(column >= 0) && */(column < numberOfColumns), "Matrix::operator()(Index, Index): request of invalid column");

		return data[row*numberOfColumns + column];
	}


	//Referencing constant access-operator on element using row- and column-values, WARNING: ZERO-BASED (DIFFERENT TO HOTINT1)
	const T& operator()(Index row, Index column) const
	{
		CHECKandTHROW(/*(row >= 0) && */(row < numberOfRows), "Matrix::operator()(Index, Index) const: request of invalid row");
		CHECKandTHROW(/*(column >= 0) && */(column < numberOfColumns), "Matrix::operator()(Index, Index) const: request of invalid column");

		return data[row*numberOfColumns + column];
	};

	//! comparison operator, component-wise compare; MATRIX DIMENSIONS MUST BE SAME; returns true, if all components are equal
	bool operator== (const ConstSizeMatrixBase& matrix) const
	{
		CHECKandTHROW((NumberOfRows() == matrix.NumberOfRows() && NumberOfColumns() == matrix.NumberOfColumns()), "Matrix::operator==: incompatible number of rows and/or columns");
		Index cnt = 0;
		for (const auto item : matrix)
		{
			if (item != data[cnt++]) { return false; }
		}
		return true;
	}

	//! add matrix to *this matrix (for each component); both matrices must have same size; FAST / no memory allocation
	ConstSizeMatrixBase& operator+= (const ConstSizeMatrixBase& matrix)
	{
		CHECKandTHROW((NumberOfRows() == matrix.NumberOfRows() && NumberOfColumns() == matrix.NumberOfColumns()), "Matrix::operator+=: incompatible number of rows and/or columns");
		Index cnt = 0;
		for (auto item : matrix) { data[cnt++] += item; }
		return *this;
	}

	//! add matrix from *this matrix (for each component); both matrices must have same size; FAST / no memory allocation
	ConstSizeMatrixBase& operator-= (const ConstSizeMatrixBase& matrix)
	{
		CHECKandTHROW((NumberOfRows() == matrix.NumberOfRows() && NumberOfColumns() == matrix.NumberOfColumns()), "Matrix::operator-=: incompatible number of rows and/or columns");
		Index cnt = 0;
		for (auto item : matrix) { data[cnt++] -= item; }
		return *this;
	}

	//! scalar multiply matrix *this with scalar (for each component); FAST / no memory allocation
	ConstSizeMatrixBase& operator*= (T scalar)
	{
		for (auto &item : *this) { item *= scalar; }
		return *this;
	}

	//! scalar divide matrix *this with scalar (for each component); no check against division by zero; FAST / no memory allocation
	ConstSizeMatrixBase& operator/= (T scalar)
	{
		for (auto &item : *this) { item /= scalar; }
		return *this;
	}


	//! add two matrices m1 and m2 (for each component); creates new ConstSizeMatrixBase / no memory allocation
	friend ConstSizeMatrixBase<T, dataSize> operator+ (const ConstSizeMatrixBase& m1, const ConstSizeMatrixBase& m2)
	{
		CHECKandTHROW(m1.NumberOfColumns() == m2.NumberOfColumns() && m1.NumberOfRows() == m2.NumberOfRows(),
			"operator+(ConstSizeMatrixBase,ConstSizeMatrixBase): Size mismatch");

		ConstSizeMatrixBase<T, dataSize> result(m1.NumberOfColumns(), m1.NumberOfRows());
		Index cnt = 0;
		for (auto &item : result)
		{
			item = m1.GetItem(cnt) + m2.GetItem(cnt);
			cnt++; //do not put cnt++ in above line, because order of summation may be interchanged ==> wrong ++ !!!
		}
		return result;
	}

	//! subtract matrix m2 from m1 (for each component); creates new ConstSizeMatrixBase / no memory allocation
	friend ConstSizeMatrixBase<T, dataSize> operator- (const ConstSizeMatrixBase& m1, const ConstSizeMatrixBase& m2)
	{
		CHECKandTHROW(m1.NumberOfColumns() == m2.NumberOfColumns() && m1.NumberOfRows() == m2.NumberOfRows(),
			"operator+(ConstSizeMatrixBase,ConstSizeMatrixBase): Size mismatch");

		ConstSizeMatrixBase<T, dataSize> result(m1.NumberOfColumns(), m1.NumberOfRows());
		Index cnt = 0;
		for (auto &item : result)
		{
			item = m1.GetItem(cnt) - m2.GetItem(cnt);
			cnt++; //do not put cnt++ in above line, because order of summation may be interchanged ==> wrong ++ !!!
		}
		return result;
	}

	//! multiply matrix m1*m2 (matrix multiplication); algorithm has order O(n^3); creates new ConstSizeMatrixBase / no memory allocation
	friend ConstSizeMatrixBase<T, dataSize> operator* (const ConstSizeMatrixBase& m1, const ConstSizeMatrixBase& m2)
	{
		CHECKandTHROW(m1.NumberOfColumns() == m2.NumberOfRows(),
			"operator*(ConstSizeMatrixBase,ConstSizeMatrixBase): Size mismatch");
		//CHECKandTHROW((m1.NumberOfRows()*m2.NumberOfColumns() <= dataSize),
		//	"ConstSizeMatrixBase::operator*(const ConstSizeMatrixBase&, const ConstSizeMatrixBase&): dataSize too small");

		ConstSizeMatrixBase<T, dataSize> result(m1.NumberOfRows(), m2.NumberOfColumns());

		for (Index i = 0; i < m2.NumberOfColumns(); i++)
		{
			for (Index j = 0; j < m1.NumberOfRows(); j++)
			{
				T value = 0;
				for (Index k = 0; k < m1.NumberOfColumns(); k++)
				{
					value += m1(j, k)*m2(k, i);
				}
				result(j, i) = value;
			}
		}
		return result;
	}

	//! multiply matrix with scalar value; creates new ConstSizeMatrixBase / no memory allocation
	friend ConstSizeMatrixBase operator* (const ConstSizeMatrixBase& matrix, const T& value)
	{
		ConstSizeMatrixBase<T, dataSize> result = matrix;
		result *= value;
		return result;
	}

	//! multiply scalar value with matrix; creates new ConstSizeMatrixBase / no memory allocation
	friend ConstSizeMatrixBase operator* (const T& value, const ConstSizeMatrixBase& matrix)
	{
		ConstSizeMatrixBase<T, dataSize> result = matrix;
		result *= value;
		return result;
	}

	//does not work!!!
	//template<class TVector, Index dataSize2>
	//friend SlimVectorBase<T, dataSize2> operator*(const ConstSizeMatrixBase& matrix, const TVector& vector)

	//leads to problems, would also be used for 4x3 matrices!!!
	//friend SlimVectorBase<T, 3> operator*(const ConstSizeMatrixBase& matrix, const SlimVectorBase<T, 3>& vector)


	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// EXTENDED FUNCTIONS
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! computes and returns the transposed of *this (does not change *this)
	ConstSizeMatrixBase<T, dataSize> GetTransposed() const
	{
		ConstSizeMatrixBase<T, dataSize> result(this->numberOfColumns, this->numberOfRows);

		for (Index i = 0; i < this->numberOfRows; i++) {
			for (Index j = 0; j < this->numberOfColumns; j++) {
				result(j, i) = (*this)(i, j);
			}
		}
		return result;
	}

	//! get fast inverse for 1D, 2D and 3D case
	ConstSizeMatrixBase<T, dataSize> GetInverse() const
	{
		CHECKandTHROW(this->numberOfColumns <= 3 && this->numberOfColumns == this->numberOfRows, "ConstSizeMatrixBase::GetInverse(): only implemented for dimensions (1x1, 2x2 and 3x3)");

		switch (this->numberOfColumns)
		{
		case 1:
		{
			T x = (*this)(0, 0);
			CHECKandTHROW(x != 0, "Matrix1D::Invert: matrix is singular");
			return ConstSizeMatrixBase<T, dataSize>(1, 1, { (T)1. / x });
			break;
		}
		case 2:
		{
			//m=[a b; c d]
			//minv = 1/(ad-bc)[d -b; -c a]
			ConstSizeMatrixBase<T, dataSize> result(2, 2);
			T det = ((*this)(0, 0)*(*this)(1, 1) - (*this)(0, 1)*(*this)(1, 0));
			CHECKandTHROW(det != 0, "Matrix2D::Invert: matrix is singular");

			T invdet = (T)1 / det;

			result(0, 0) = invdet * (*this)(1, 1);
			result(0, 1) = -invdet * (*this)(0, 1);
			result(1, 0) = -invdet * (*this)(1, 0);
			result(1, 1) = invdet * (*this)(0, 0);
			return result;
			break;
		}
		case 3:
		{
			const ConstSizeMatrixBase<T, dataSize>& m = *this;
			T det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) - m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) + m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
			CHECKandTHROW(det != 0, "Matrix3D::Invert: matrix is singular");

			T invdet = (T)1 / det;

			ConstSizeMatrixBase<T, dataSize> result(3, 3); // inverse of matrix m
			result(0, 0) = invdet * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2));
			result(0, 1) = invdet * (m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2));
			result(0, 2) = invdet * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1));
			result(1, 0) = invdet * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2));
			result(1, 1) = invdet * (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0));
			result(1, 2) = invdet * (m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2));
			result(2, 0) = invdet * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1));
			result(2, 1) = invdet * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1));
			result(2, 2) = invdet * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1));
			return result;
			break;
		}
		default: //may not occur due to static assertion
			return *this;
		}
	}

	//! @brief Output operator << generates ostream "[m[0][0] m[0][1] ... m[0][m]; ... m[n][m]]" for a matrix m;
	//! the FORMAT IS DIFFERENT TO HOTINT1
	//! @todo check Vector/Matrix ostream output format to be compatible with Python / NumPy
	friend std::ostream& operator<<(std::ostream& os, const ConstSizeMatrixBase& matrix)
	{
		if (linalgPrintUsePythonFormat)
		{
			os << "[";
			for (Index row = 0; row < matrix.NumberOfRows(); row++)
			{
				os << "[";
				for (Index column = 0; column < matrix.NumberOfColumns(); column++)
				{
					os << matrix(row, column);
					if (column != matrix.NumberOfColumns() - 1) { os << ","; }
				}
				os << "]";
				if (row != matrix.NumberOfRows() - 1) { os << ","; }
			}
			os << "]";
		}
		else
		{
			os << "[";
			for (Index row = 0; row < matrix.NumberOfRows(); row++)
			{
				for (Index column = 0; column < matrix.NumberOfColumns(); column++)
				{
					os << matrix(row, column);
					if (column != matrix.NumberOfColumns() - 1) { os << " "; }
				}
				if (row != matrix.NumberOfRows() - 1) { os << "; "; } //';' and '[]' as a compromise between Python and MATLAB format
			}
			os << "]";
		}
		return os;
	}


	//! transposes *this; currently only works for square matrix
	//! @todo check efficient implementation of tranpose for non-square matrices
	void TransposeYourself()
	{
		CHECKandTHROW(IsSquare(), "Matrix::GetTransposed: matrix must be square!");

		for (Index i = 0; i < numberOfRows; i++) {
			for (Index j = 0; j < i; j++) { //operates only on lower left triangular matrix
				EXUstd::Swap((*this)(i, j), (*this)(j, i));
			}
		}
	}

	//! return whether *this is a square matrix (numberOfRows == numberOfColumns)
	bool IsSquare() const { return numberOfRows == numberOfColumns; }

	//! get column vector as a SlimVector (no memory allocation, but final size needs to be known at compile time)
	//! use e.g.: Vector3D v = GetColumnVector<3>(i);
	template<Index columnSize>
	SlimVectorBase<T, columnSize> GetColumnVector(Index column) const
	{
		CHECKandTHROW(this->numberOfRows == columnSize,
			"ConstSizeMatrixBase::GetColumnVector(...): size mismatch");
		CHECKandTHROW(column <= this->numberOfColumns,
			"ConstSizeMatrixBase::GetColumnVector(...): illegal column");

		SlimVectorBase<T, columnSize> result;

		for (Index i = 0; i < this->numberOfRows; i++)
		{
			result[i] = (*this)(i, column);
		}
		return result;
	}

	SlimVectorBase<T, 3> GetColumnVector(Index column) const
	{
		CHECKandTHROW(this->numberOfRows == 3,
			"ConstSizeMatrixBase::GetColumnVector(...): size mismatch");
		CHECKandTHROW(column <= this->numberOfColumns,
			"ConstSizeMatrixBase::GetColumnVector(...): illegal column");

		SlimVectorBase<T, 3> result;

		for (Index i = 0; i < this->numberOfRows; i++)
		{
			result[i] = (*this)(i, column);
		}
		return result;
	}

};



#else

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++






template <typename T, Index dataSize>
class ConstSizeMatrixBase : public MatrixBase<T>
{
private:
	T constData[dataSize];
public:

	//Constructors, Destructor

	//! create empty (0 x 0) matrix, default constructor, rule of 5
	ConstSizeMatrixBase()
	{
		this->data = &constData[0]; //constructor should always start with data linkage
									//use this-> to access member variables, because of templated base class
		this->numberOfRows = 0;
		this->numberOfColumns = 0;
	}

	//! copy constructor, based on iterators, rule of 5
	ConstSizeMatrixBase(const ConstSizeMatrixBase& matrix)
	{
		this->data = &constData[0];

		//not needed, because only with same matrices possible!
		//CHECKandTHROW((matrix.numberOfRows*matrix.numberOfColumns <= dataSize),
		//	"ConstSizeMatrixBase::ConstSizeMatrixBase(const ConstSizeMatrixBase& matrix): dataSize too small");

		ResizeMatrix(matrix.NumberOfRows(), matrix.NumberOfColumns());

		Index cnt = 0;
		for (auto value : matrix) { this->data[cnt++] = value; }
	}

	//! move constructor, rule of 5
	//ConstSizeMatrixBase(ConstSizeMatrixBase&& other) noexcept
	//{
	//	//exchange of pointers will not work! 
	//	//this->data = std::exchange(other.data, nullptr);
	//	//this->numberOfRows = std::exchange(other.numberOfRows, 0);
	//	//this->numberOfColumns = std::exchange(other.numberOfColumns, 0);
	//	for (Index i = 0; i < other.numberOfRows*other.numberOfColumns; i++) {
	//		constData[i] = std::exchange(other.constData[i], (T)0);
	//	}
	//	this->numberOfRows = std::exchange(other.numberOfRows, 0);
	//	this->numberOfColumns = std::exchange(other.numberOfColumns, 0);
	//}

	//! create matrix with dimensions numberOfRowsInit x numberOfColumnsInit; data is not initialized
	ConstSizeMatrixBase(Index numberOfRowsInit, Index numberOfColumnsInit)
	{
		this->data = &constData[0];

		CHECKandTHROW((numberOfRowsInit >= 0 && numberOfColumnsInit >= 0 && numberOfRowsInit*numberOfColumnsInit <= dataSize),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(Index, Index): invalid parameters");

		ResizeMatrix(numberOfRowsInit, numberOfColumnsInit);
	}

	//! create matrix with dimensions numberOfRowsInit x numberOfColumnsInit; initialize items with 'initializationValue'
	ConstSizeMatrixBase(Index numberOfRowsInit, Index numberOfColumnsInit, T initializationValue)
    {
		this->data = &constData[0];
		
		CHECKandTHROW((numberOfRowsInit >= 0 && numberOfColumnsInit >= 0 && numberOfRowsInit*numberOfColumnsInit <= dataSize),
            "ConstSizeMatrixBase::ConstSizeMatrixBase(Index, Index, T): invalid parameters");
		CHECKandTHROW((initializationValue == 0), "ConstSizeMatrixBase: initializationValue != 0"); 

        ResizeMatrix(numberOfRowsInit, numberOfColumnsInit);

        for (auto &item : *this) {
            item = initializationValue;
        }
    }

    //! create matrix with dimensions numberOfRowsInit x numberOfColumnsInit; initialize data with initializer list
	ConstSizeMatrixBase(Index numberOfRowsInit, Index numberOfColumnsInit, std::initializer_list<T> listOfReals)
    {
		this->data = &constData[0];
		
		CHECKandTHROW((numberOfRowsInit*numberOfColumnsInit == (Index)listOfReals.size()),
                       "ConstSizeMatrixBase::ConstSizeMatrixBase(Index, Index, initializer_list): inconsistent size of initializer_list");

        ResizeMatrix(numberOfRowsInit, numberOfColumnsInit);

        Index cnt = 0;
        for (auto value : listOfReals) {
			this->data[cnt++] = value;
        }
    }

	//! copy constructor with MatrixBase; @todo: check if ConstSizeMatrixBase(const MatrixBase& matrix) is NEEDED?
	ConstSizeMatrixBase(const MatrixBase<T>& matrix)
	{
		this->data = &constData[0];

		CHECKandTHROW((matrix.NumberOfRows()*matrix.NumberOfColumns() <= dataSize),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(const MatrixBase& matrix): dataSize too small");

		ResizeMatrix(matrix.NumberOfRows(), matrix.NumberOfColumns());

		Index cnt = 0;
		for (auto value : matrix) { this->data[cnt++] = value; }
	}

	//! copy from std::array<std::array<Real,nCols>,nRows> matrix; gcc-7 having problems with that!
	template<Index matrixColumns, Index matrixRows>
	ConstSizeMatrixBase(const std::array<std::array<T, matrixColumns>, matrixRows>& matrix)
	{
		this->data = &constData[0];

		CHECKandTHROW((matrixRows*matrixColumns <= dataSize),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(const std::array<std::array<T, matrixColumns>, matrixRows>& matrix): dataSize too small");

		ResizeMatrix(matrixRows, matrixColumns);

		Index cnt = 0;
		for (Index rows = 0; rows < matrixRows; rows++)
		{
			for (Index cols = 0; cols < matrixColumns; cols++)
			{
				constData[cnt++] = matrix[rows][cols];
			}
		}
	}

	ConstSizeMatrixBase(const StdArray33F& matrix)
	{
		this->data = &constData[0];
		CHECKandTHROW((dataSize == 9),
			"ConstSizeMatrixBase::ConstSizeMatrixBase(const StdArray33F& matrix& matrix): dataSize invalid");

		ResizeMatrix(3,3);

		Index cnt = 0;
		for (Index rows = 0; rows < 3; rows++)
		{
			for (Index cols = 0; cols < 3; cols++)
			{
				constData[cnt++] = matrix[rows][cols];
			}
		}
	}

	virtual ~ConstSizeMatrixBase<T, dataSize>()
    {
		this->data = nullptr; //MatrixBase destructor called hereafter
		//do nothing, because no memory allocated
    };


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // BASIC FUNCTIONS
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

protected:
	virtual Index MaxAllocatedSize() const { return dataSize; }
	virtual bool IsConstSizeMatrix() const  override { return true; }

	//! Initialization of data ==> should never be called in ConstSizeMatrixBase; @todo: delete after tests finished
	virtual void Init() override
	{
		this->numberOfRows = 0;
		this->numberOfColumns = 0;
		CHECKandTHROWstring("ConstSizeMatrixBase::Init(): should never be called");
	};

    //! Set new size of matrix; for external access, use 'SetNumberOfRowsAndColumns' to modify size of matrix
    virtual void ResizeMatrix(Index numberOfRowsInit, Index numberOfColumnsInit) override
    {
		//checks done already in caller function!
		//CHECKandTHROW((numberOfRowsInit*numberOfColumnsInit <= dataSize),
		//	"ConstSizeMatrixBase::ResizeMatrix(Index,Index): dataSize too small");
		
		this->numberOfRows = numberOfRowsInit;
		this->numberOfColumns = numberOfColumnsInit;
    }

public:
	//Assignment-operator, copy only active data from matrix
	ConstSizeMatrixBase<T, dataSize>& operator= (const ConstSizeMatrixBase<T, dataSize>& matrix)
	{
		if (this == &matrix) { return *this; }

		//not needed, because requirements always fulfilled!:
		//CHECKandTHROW((this->numberOfRows*this->numberOfColumns <= dataSize),
		//	"ConstSizeMatrixBase::operator=: dataSize too small");

		ResizeMatrix(matrix.NumberOfRows(), matrix.NumberOfColumns());

		Index cnt = 0;
		for (auto value : matrix) { this->data[cnt++] = value; }

		return *this;
	}

	//! move assignment operator, rule of 5 (but does not work in such a way)
	//leads immediatly to crash
	//ConstSizeMatrixBase<T, dataSize>& operator= (ConstSizeMatrixBase<T, dataSize>&& other) = default;
	//ConstSizeMatrixBase<T, dataSize>& operator= (ConstSizeMatrixBase<T, dataSize>&& other) noexcept
	//{
	//	std::swap(*this, other); //seems to work for static data?
	//	return *this;

	//	//std::swap(this->numberOfRows, other.numberOfRows);
	//	//std::swap(this->numberOfColumns, other.numberOfColumns);
	//	//return *this;
	//}

	//! add two matrices m1 and m2 (for each component); creates new ConstSizeMatrixBase / no memory allocation
	friend ConstSizeMatrixBase<T,dataSize> operator+ (const ConstSizeMatrixBase& m1, const ConstSizeMatrixBase& m2)
	{
		CHECKandTHROW(m1.NumberOfColumns() == m2.NumberOfColumns() && m1.NumberOfRows() == m2.NumberOfRows(),
			"operator+(ConstSizeMatrixBase,ConstSizeMatrixBase): Size mismatch");
		//CHECKandTHROW((m1.NumberOfColumns()*m1.NumberOfRows() <= dataSize),
		//	"ConstSizeMatrixBase::operator+(const ConstSizeMatrixBase&, const ConstSizeMatrixBase&): dataSize too small");

		ConstSizeMatrixBase<T, dataSize> result(m1.NumberOfColumns(), m1.NumberOfRows());
		Index cnt = 0;
		for (auto &item : result)
		{
			item = m1.GetItem(cnt) + m2.GetItem(cnt);
			cnt++; //do not put cnt++ in above line, because order of summation may be interchanged ==> wrong ++ !!!
		}
		return result;
	}

	//! subtract matrix m2 from m1 (for each component); creates new ConstSizeMatrixBase / no memory allocation
	friend ConstSizeMatrixBase<T, dataSize> operator- (const ConstSizeMatrixBase& m1, const ConstSizeMatrixBase& m2)
	{
		CHECKandTHROW(m1.NumberOfColumns() == m2.NumberOfColumns() && m1.NumberOfRows() == m2.NumberOfRows(),
			"operator+(ConstSizeMatrixBase,ConstSizeMatrixBase): Size mismatch");
		//CHECKandTHROW((m1.NumberOfColumns()*m1.NumberOfRows() <= dataSize),
		//	"ConstSizeMatrixBase::operator+(const ConstSizeMatrixBase&, const ConstSizeMatrixBase&): dataSize too small");

		ConstSizeMatrixBase<T, dataSize> result(m1.NumberOfColumns(), m1.NumberOfRows());
		Index cnt = 0;
		for (auto &item : result)
		{
			item = m1.GetItem(cnt) - m2.GetItem(cnt);
			cnt++; //do not put cnt++ in above line, because order of summation may be interchanged ==> wrong ++ !!!
		}
		return result;
	}

    //! multiply matrix m1*m2 (matrix multiplication); algorithm has order O(n^3); creates new ConstSizeMatrixBase / no memory allocation
    friend ConstSizeMatrixBase<T, dataSize> operator* (const ConstSizeMatrixBase& m1, const ConstSizeMatrixBase& m2)
    {
        CHECKandTHROW(m1.NumberOfColumns() == m2.NumberOfRows(),
            "operator*(ConstSizeMatrixBase,ConstSizeMatrixBase): Size mismatch");
		//CHECKandTHROW((m1.NumberOfRows()*m2.NumberOfColumns() <= dataSize),
		//	"ConstSizeMatrixBase::operator*(const ConstSizeMatrixBase&, const ConstSizeMatrixBase&): dataSize too small");

        ConstSizeMatrixBase<T, dataSize> result(m1.NumberOfRows(), m2.NumberOfColumns());

        for (Index i = 0; i < m2.NumberOfColumns(); i++) 
        {
            for (Index j = 0; j < m1.NumberOfRows(); j++)
            {
                T value = 0;
                for (Index k = 0; k < m1.NumberOfColumns(); k++)
                {
                    value += m1(j, k)*m2(k,i);
                }
                result(j,i) = value;
            }
        }
        return result;
    }

    //! multiply matrix with scalar value; creates new ConstSizeMatrixBase / no memory allocation
    friend ConstSizeMatrixBase operator* (const ConstSizeMatrixBase& matrix, const T& value)
    {
        ConstSizeMatrixBase<T, dataSize> result = matrix;
        result *= value;
        return result;
    }

    //! multiply scalar value with matrix; creates new ConstSizeMatrixBase / no memory allocation
    friend ConstSizeMatrixBase operator* (const T& value, const ConstSizeMatrixBase& matrix)
    {
		ConstSizeMatrixBase<T, dataSize> result = matrix;
		result *= value;
		return result;
	}

	//does not work!!!
	//template<class TVector, Index dataSize2>
	//friend SlimVectorBase<T, dataSize2> operator*(const ConstSizeMatrixBase& matrix, const TVector& vector)

	//leads to problems, would also be used for 4x3 matrices!!!
	//friend SlimVectorBase<T, 3> operator*(const ConstSizeMatrixBase& matrix, const SlimVectorBase<T, 3>& vector)


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // EXTENDED FUNCTIONS
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //! computes and returns the transposed of *this (does not change *this)
    virtual ConstSizeMatrixBase<T, dataSize> GetTransposed() const
    {
        ConstSizeMatrixBase<T, dataSize> result(this->numberOfColumns, this->numberOfRows);

        for (Index i = 0; i < this->numberOfRows; i++) {
            for (Index j = 0; j < this->numberOfColumns; j++) {
                result(j,i) = (*this)(i,j);
            }
        }
        return result;
    }

	//! get fast inverse for 1D, 2D and 3D case
	virtual ConstSizeMatrixBase<T, dataSize> GetInverse() const
	{
		CHECKandTHROW(this->numberOfColumns <= 3 && this->numberOfColumns == this->numberOfRows, "ConstSizeMatrixBase::GetInverse(): only implemented for dimensions (1x1, 2x2 and 3x3)");

		switch (this->numberOfColumns)
		{
			case 1: 
			{
				T x = (*this)(0, 0);
				CHECKandTHROW(x != 0, "Matrix1D::Invert: matrix is singular");
				return ConstSizeMatrixBase<T, dataSize>(1, 1, { (T)1. / x });
				break;
			}
			case 2: 
			{
				//m=[a b; c d]
				//minv = 1/(ad-bc)[d -b; -c a]
				ConstSizeMatrixBase<T, dataSize> result(2, 2);
				T det = ((*this)(0, 0)*(*this)(1, 1) - (*this)(0, 1)*(*this)(1, 0));
				CHECKandTHROW(det != 0, "Matrix2D::Invert: matrix is singular");

				T invdet = (T)1 / det;

				result(0, 0) = invdet * (*this)(1, 1);
				result(0, 1) =-invdet * (*this)(0, 1);
				result(1, 0) =-invdet * (*this)(1, 0);
				result(1, 1) = invdet * (*this)(0, 0);
				return result;
				break;
			}
			case 3:
			{
				const ConstSizeMatrixBase<T, dataSize>& m = *this;
				T det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) - m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) + m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
				CHECKandTHROW(det != 0, "Matrix3D::Invert: matrix is singular");

				T invdet = (T)1 / det;

				ConstSizeMatrixBase<T, dataSize> result(3,3); // inverse of matrix m
				result(0, 0) = invdet * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2));
				result(0, 1) = invdet * (m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2));
				result(0, 2) = invdet * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1));
				result(1, 0) = invdet * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2));
				result(1, 1) = invdet * (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0));
				result(1, 2) = invdet * (m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2));
				result(2, 0) = invdet * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1));
				result(2, 1) = invdet * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1));
				result(2, 2) = invdet * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1));
				return result;
				break;
			}
			default: //may not occur due to static assertion
				return *this;
		}
	}

};

#endif

//multiplication must be defined outside and with "9" ConstSizeMatrixBase<T, 9>, otherwise this operator is also used for 4x3 matrices
template<typename T>
SlimVectorBase<T, 3> operator*(const ConstSizeMatrixBase<T, 9>& matrix, const SlimVectorBase<T, 3>& vector)
{
	CHECKandTHROW(matrix.NumberOfColumns() == vector.NumberOfItems(),
		"operator*(ConstSizeMatrixBase,SlimVectorBase<T, 3>): Size mismatch");
	CHECKandTHROW((matrix.NumberOfRows() == 3),
		"operator*(ConstSizeMatrixBase,SlimVectorBase<T, 3>): matrix does not fit");

	SlimVectorBase<T, 3> result; //no initialization for SlimVector

	for (Index i = 0; i < result.NumberOfItems(); i++)
	{
		T resultRow = 0;
		for (Index j = 0; j < vector.NumberOfItems(); j++)
		{
			resultRow += matrix(i, j)*vector[j];
		}
		result[i] = resultRow;
	}
	return result;
}

//multiplication must be defined outside and with "9" ConstSizeMatrixBase<T, 9>, otherwise this operator is also used for 4x3 matrices
template<typename T>
SlimVectorBase<T, 3> operator*(const SlimVectorBase<T, 3>& vector, const ConstSizeMatrixBase<T, 9>& matrix)
{
	CHECKandTHROW(matrix.NumberOfRows() == vector.NumberOfItems(),
		"operator*(SlimVectorBase<T, 3>,ConstSizeMatrixBase): Size mismatch");
	CHECKandTHROW((matrix.NumberOfColumns() == 3),
		"operator*(SlimVectorBase<T, 3>,ConstSizeMatrixBase): matrix does not fit");

	SlimVectorBase<T, 3> result; //no initialization for SlimVector

	for (Index i = 0; i < result.NumberOfItems(); i++)
	{
		T resultRow = 0;
		for (Index j = 0; j < vector.NumberOfItems(); j++)
		{
			resultRow += vector[j] * matrix(j, i);
		}
		result[i] = resultRow;
	}
	return result;
}

//multiplication must be defined outside and with "4" ConstSizeMatrixBase<T, 4>
template<typename T>
SlimVectorBase<T, 2> operator*(const ConstSizeMatrixBase<T, 4>& matrix, const SlimVectorBase<T, 2>& vector)
{
	CHECKandTHROW(matrix.NumberOfColumns() == vector.NumberOfItems(),
		"operator*(ConstSizeMatrixBase,SlimVectorBase<T, 2>): Size mismatch");
	CHECKandTHROW((matrix.NumberOfRows() == 2),
		"operator*(ConstSizeMatrixBase,SlimVectorBase<T, 2>): matrix does not fit");

	SlimVectorBase<T, 2> result; //no initialization for SlimVector

	for (Index i = 0; i < result.NumberOfItems(); i++)
	{
		T resultRow = 0;
		for (Index j = 0; j < vector.NumberOfItems(); j++)
		{
			resultRow += matrix(i, j)*vector[j];
		}
		result[i] = resultRow;
	}
	return result;
}

//multiplication must be defined outside and with "4" ConstSizeMatrixBase<T, 4>
template<typename T>
SlimVectorBase<T, 2> operator*(const SlimVectorBase<T, 2>& vector, const ConstSizeMatrixBase<T, 4>& matrix)
{
	CHECKandTHROW(matrix.NumberOfRows() == vector.NumberOfItems(),
		"operator*(SlimVectorBase<T, 2>,ConstSizeMatrixBase): Size mismatch");
	CHECKandTHROW((matrix.NumberOfColumns() == 2),
		"operator*(SlimVectorBase<T, 2>,ConstSizeMatrixBase): matrix does not fit");

	SlimVectorBase<T, 2> result; //no initialization for SlimVector

	for (Index i = 0; i < result.NumberOfItems(); i++)
	{
		T resultRow = 0;
		for (Index j = 0; j < vector.NumberOfItems(); j++)
		{
			resultRow += vector[j] * matrix(j, i);
		}
		result[i] = resultRow;
	}
	return result;
}


template<Index dataSize>
using ConstSizeMatrix = ConstSizeMatrixBase<Real,dataSize>;

template<Index dataSize>
using ConstSizeMatrixF = ConstSizeMatrixBase<float, dataSize>;

//not yet supported in C++ standard: typedef ConstSizeMatrixBase<Real, Index dataSize> ConstSizeMatrix<dataSize>;

//! matrix*vector multiplication with given result vector; invokes memory allocation
//template<class TVector, Index dataSize, Index dataSize2>
//template <>
//SlimVector<3> operator*(const ConstSizeVector<9>& matrix, const SlimVector<3>& vector)
//{
//	Vector3D result;
//	EXUmath::MultMatrixVector(matrix, vector, result);
//	return result;
//}
//

//{
//	CHECKandTHROW(matrix.NumberOfColumns() == vector.NumberOfItems(),
//		"operator*(Matrix,TVector): Size mismatch");
//	CHECKandTHROW((matrix.NumberOfRows() <= dataSize),
//		"operator*(Matrix,TVector): dataSize too small");
//
//	ConstSizeVector<dataSize> result(matrix.NumberOfRows());
//
//	for (Index i = 0; i < result.NumberOfItems(); i++)
//	{
//		double resultRow = 0;
//		for (Index j = 0; j < vector.NumberOfItems(); j++)
//		{
//			resultRow += matrix(i, j)*vector[j];
//		}
//		result[i] = resultRow;
//	}
//	return result;
//}

#endif

