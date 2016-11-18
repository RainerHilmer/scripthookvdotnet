/**
 * Copyright (C) 2015 crosire
 *
 * This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
 * authors be held liable for any damages arising from the use of this software.
 * Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
 *      original  software. If you use this  software  in a product, an  acknowledgment in the product
 *      documentation would be appreciated but is not required.
 *   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
 *      being the original software.
 *   3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include "NativeHashes.hpp"
#include "Vector3.hpp"

namespace GTA
{
	namespace Native
	{
		public interface class INativeValue
		{
			property System::UInt64 NativeValue
			{
				System::UInt64 get();
				void set(System::UInt64 value);
			};
		};

		[System::Runtime::InteropServices::StructLayout(System::Runtime::InteropServices::LayoutKind::Explicit, Size = 0x18)]
		private value class NativeVector3
		{
			//For natives that require pointers to Vectors and are called internally in the scripting section.
			//Use this struct and pass a pointer to it, then after the nativecall this can be casted back to a GTA.Math.Vector3
		public:
			[System::Runtime::InteropServices::FieldOffset(0x00)] float X;
			[System::Runtime::InteropServices::FieldOffset(0x08)] float Y;
			[System::Runtime::InteropServices::FieldOffset(0x10)] float Z;
			NativeVector3(float x, float y, float z):X(x), Y(y), Z(z){}
			static operator GTA::Math::Vector3(NativeVector3 value)
			{
				return GTA::Math::Vector3(value.X, value.Y, value.Z);
			}

			static operator NativeVector3(GTA::Math::Vector3 value)
			{
				return NativeVector3(value.X, value.Y, value.Z);
			}
		};

		#pragma region Functions
		public ref class InputArgument
		{
		public:
			InputArgument(System::Object ^value);

			virtual System::String ^ToString() override
			{
				return _data.ToString();
			}

			// Value types
			static operator InputArgument ^ (bool value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (char value)
			{
				return gcnew InputArgument(static_cast<int>(value));
			}
			static operator InputArgument ^ (unsigned char value)
			{
				return gcnew InputArgument(static_cast<int>(value));
			}
			static operator InputArgument ^ (short value)
			{
				return gcnew InputArgument(static_cast<int>(value));
			}
			static operator InputArgument ^ (unsigned short value)
			{
				return gcnew InputArgument(static_cast<int>(value));
			}
			static operator InputArgument ^ (int value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (unsigned int value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (long long value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (unsigned long long value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (float value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (double value)
			{
				return gcnew InputArgument(static_cast<float>(value));
			}
			static operator InputArgument ^ (System::Enum ^value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (INativeValue ^value)
			{
				return gcnew InputArgument(value);
			}

			// String types
			static operator InputArgument ^ (System::String ^value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (const char *value)
			{
				return gcnew InputArgument(gcnew System::String(value));
			}

			// Pointer types
			static operator InputArgument ^ (System::IntPtr value)
			{
				return gcnew InputArgument(value);
			}
			static operator InputArgument ^ (void *value)
			{
				return gcnew InputArgument(System::IntPtr(value));
			}

		internal:
			System::UInt64 _data;
		};
		public ref class OutputArgument : public InputArgument
		{
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="OutputArgument"/> class for natives that output data into pointers.
			/// </summary>
			OutputArgument();
			/// <summary>
			/// Initializes a new instance of the <see cref="OutputArgument"/> class with an initial value for natives that require the pointer to data instead of the actual data.
			/// </summary>
			/// <param name="initvalue">The value to set the data of this <see cref="OutputArgument"/> to.</param>
			OutputArgument(System::Object ^initvalue);
			~OutputArgument();

			/// <summary>
			/// Gets the value of data stored in this <see cref="OutputArgument"/>.
			/// </summary>
			generic <typename T>
			T GetResult();

		protected:
			!OutputArgument();

			unsigned char *_storage;
		};

		public ref class Function abstract sealed
		{
		public:
			/// <summary>
			/// Calls the specified native script function and returns its return value.
			/// </summary>
			/// <param name="hash">The hashed name of the native script function.</param>
			/// <param name="arguments">A list of input and output arguments to pass to the native script function.</param>
			/// <returns>The return value of the native</returns>
			generic <typename T>
			static T Call(Hash hash, ... array<InputArgument ^> ^arguments);
			/// <summary>
			/// Calls the specified native script function and ignores its return value.
			/// </summary>
			/// <param name="hash">The hashed name of the script function.</param>
			/// <param name="arguments">A list of input and output arguments to pass to the native script function.</param>
			static void Call(Hash hash, ... array<InputArgument ^> ^arguments);

		internal:
			generic <typename T>
			static T Call(System::UInt64 hash, ... array<InputArgument ^> ^arguments);
		};
		#pragma endregion

		#pragma region Global Variables
		public value class GlobalVariable sealed
		{
		public:
			/// <summary>
			/// Gets the global variable at the specified index.
			/// </summary>
			/// <param name="index">The index of the global variable.</param>
			/// <returns>A <see cref="GlobalVariable"/> instance representing the global variable.</returns>
			static GlobalVariable Get(int index);

			/// <summary>
			/// Gets the native memory address of the <see cref="GlobalVariable"/>.
			/// </summary>
			property System::IntPtr MemoryAddress
			{
				System::IntPtr get()
				{
					return _address;
				}
			}

			/// <summary>
			/// Gets the value stored in the <see cref="GlobalVariable"/>.
			/// </summary>
			generic <typename T>
			T Read();
			/// <summary>
			/// Set the value stored in the <see cref="GlobalVariable"/>.
			/// </summary>
			/// <param name="value">The new value to assign to the <see cref="GlobalVariable"/>.</param>
			generic <typename T>
			void Write(T value);
			/// <summary>
			/// Set the value stored in the <see cref="GlobalVariable"/> to a string.
			/// </summary>
			/// <param name="value">The string to set the <see cref="GlobalVariable"/> to.</param>
			/// <param name="maxSize">The maximum size of the string. Can be found for a given global variable by checking the decompiled scripts from the game.</param>
			void WriteString(System::String ^value, int maxSize);

			/// <summary>
			/// Set the value of a specific bit of the <see cref="GlobalVariable"/> to true.
			/// </summary>
			/// <param name="index">The zero indexed bit of the <see cref="GlobalVariable"/> to set.</param>
			void SetBit(int index);
			/// <summary>
			/// Set the value of a specific bit of the <see cref="GlobalVariable"/> to false.
			/// </summary>
			/// <param name="index">The zero indexed bit of the <see cref="GlobalVariable"/> to clear.</param>
			void ClearBit(int index);
			/// <summary>
			/// Gets a value indicating whether a specific bit of the <see cref="GlobalVariable"/> is set.
			/// </summary>
			/// <param name="index">The zero indexed bit of the <see cref="GlobalVariable"/> to check.</param>
			bool IsBitSet(int index);

			/// <summary>
			/// Gets the <see cref="GlobalVariable"/> stored at a given offset in a global structure.
			/// </summary>
			/// <param name="index">The index the <see cref="GlobalVariable"/> is stored in the structure. For example the Y component of a Vector3 is at index 1.</param>
			/// <returns>The <see cref="GlobalVariable"/> at the index given.</returns>
			GlobalVariable GetStructField(int index);

			/// <summary>
			/// Returns an array of all <see cref="GlobalVariable"/>s in a global array.
			/// </summary>
			/// <param name="itemSize">The number of items stored in each array index. For example an array of Vector3s takes up 3 items.</param>
			/// <returns>The array of <see cref="GlobalVariable"/>s.</returns>
			array<GlobalVariable> ^GetArray(int itemSize);
			/// <summary>
			/// Gets the <see cref="GlobalVariable"/> stored at a specific index in a global array.
			/// </summary>
			/// <param name="index">The array index.</param>
			/// <param name="itemSize">The number of items stored in each array index. For example an array of Vector3s takes up 3 items.</param>
			/// <returns>The <see cref="GlobalVariable"/> at the index given.</returns>
			GlobalVariable GetArrayItem(int index, int itemSize);

		private:
			GlobalVariable(System::IntPtr address);

			System::IntPtr _address;
		};
		#pragma endregion
	}
}
