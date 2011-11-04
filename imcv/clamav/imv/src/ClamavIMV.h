/* 
 * Copyright (C) 2006-2011 Fachhochschule Hannover
 * (University of Applied Sciences and Arts, Hannover)
 * Faculty IV, Dept. of Computer Science
 * Ricklinger Stadtweg 118, 30459 Hannover, Germany
 * 
 * Email: trust@f4-i.fh-hannover.de
 * Website: http://trust.inform.fh-hannover.de/
 * 
 * This file is part of tnc@fhh, an open source 
 * Trusted Network Connect implementation by the Trust@FHH
 * research group at the Fachhochschule Hannover.
 * 
 * tnc@fhh is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * tnc@fhh is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with tnc@fhh; if not, see <http://www.gnu.org/licenses/>
 */
 
#ifndef CLAMAVIMV_H_
#define CLAMAVIMV_H_

#include <imunit/imv/AbstractIMV.h>
#include "ClamavIMVLibrary.h"

#include <vector>

// file is generated by calling cmake
#include <clamavimvConfig.h>

/**
 * ClamavIMV.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>11.01.2010 - create class (ib)</li>
 * </ul>
 *
 * @date 11.01.2010
 * @author Ingo Bente (ib)
 */
class ClamavIMV: public tncfhh::iml::AbstractIMV {
public:
	/**
	 * Ctor.
	 */
	ClamavIMV(TNC_ConnectionID conID, ClamavIMVLibrary *pClamavImvLibrary);

	/**
	 * Dtor.
	 */
	virtual ~ClamavIMV();

	// ===================
	// methods to override
	// ===================

	/**
	 * notifyConnectionChange
	 */
	virtual TNC_Result notifyConnectionChange();

	/**
	 * receiveMessage
	 */
	virtual TNC_Result receiveMessage(TNC_BufferReference message,
			                          TNC_UInt32 messageLength,
			                          TNC_MessageType messageType);

	/**
	 * batchEnding
	 */
	virtual TNC_Result batchEnding();

protected:

	/**
	 * Enum for valid policy keys
	 */
	enum Key{
		OperationalStatus,
		Version,
		Main_Version,
		Daily_Version
	};

	/**
	 * Enum for valid policy operants
	 */
	enum Operant{
		eq,
		gt,
		lt,
		gte,
		lte,
		uneq
	};

	/**
	 * Class wrapping a policy entry. Contains Key, Operant and String value.
	 */
	class PolicyEntry{
	public:
		Key key;
		Operant operant;
		std::string value;

		PolicyEntry(Key key, Operant operant, std::string value);
	};

	class Measurement {
	public:
		Key key;
		std::string keyAsString;
		std::string value;

		Measurement(Key key, std::string keyAsString, std::string value);
	};

	/**
	 * Parses the policy of the ClamavIMV. All specified entries are saved
	 * in the corresponding member variable. The policy parsing takes
	 * place at the beginning of each new handshake.
	 */
	void parsePolicy();

	/**
	 * Saves a policy entry in the corresponding member variable.
	 * Is called by parsePolicy().
	 */
	void savePolicyEntry(std::string key, std::string operant, std::string value);

	/**
	 * Container for all policy entries.
	 */
	std::vector<PolicyEntry> policyEntries;

	private:
	TNC_IMV_Action_Recommendation doLocalEvaluation(std::vector<Measurement>* measurements);
	bool doXACMLEvaluation(std::vector<Measurement>* measurements);
	bool compareVersionStrings(std::string in, std::string policy);
};

#endif /* CLAMAVIMV_H_ */