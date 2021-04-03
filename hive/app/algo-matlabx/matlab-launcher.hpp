/*
 *  ::718604!
 * 
 * Copyright(C) November 20, 2014 U.S. Food and Drug Administration
 * Authors: Dr. Vahan Simonyan (1), Dr. Raja Mazumder (2), et al
 * Affiliation: Food and Drug Administration (1), George Washington University (2)
 * 
 * All rights Reserved.
 * 
 * The MIT License (MIT)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once
#ifndef sDnaML_hpp
#define sDnaML_hpp

#include <slib/core/dic.hpp>
#include <slib/std/file.hpp>
#include <violin/violin.hpp>
#include <qlib/QPrideProc.hpp>
#include <dmlib/dmlib.hpp>

using namespace slib;


class MatlabLauncherX
{
    public:

    /**
     * @brief Status returns for Matlab processes
     * 
     */
    typedef enum
    {
        eProcessMessage_Wait = -1,
        eProcessMessage_Error = 0,
        eProcessMessage_Started,
        eProcessMessage_Done,
        eProcessMessage_Aborted
    } EProcessMessage;

    /**
     * @brief Construct a new Matlab Launcher X object
     * 
     * @param qprideproc 
     */
    MatlabLauncherX(sQPrideProc & qprideproc) : qp(qprideproc) { };
    
    /**
     * @brief Destroy the Matlab Launcher X object
     * 
     */
    virtual ~MatlabLauncherX();

    /**
     * @brief Populate parameters within the object.
     * 
     * Reads the parameters from the HTML form.  Records a copy of this within the object for access later.
     * 
     * @param pForm The HTML form sent from the front end.  This object contains all of the entries as specified in the database.
     * @param user The user object.
     * @param objs Vector of user process objects.
     */
    virtual void readParams(sVar * pForm, sUsr * user, sVec<sUsrProc> & objs);
    

    sQPrideProc & qp;


};

class SignalPredictor: public MatlabLauncherX
{

};

class DnaMLX
{
    public:
        typedef enum
        {
            eIndexerMessage_Wait = -1,
            eIndexerMessage_Error = 0,
            EIndexerMessage_Started,
            eIndexerMessage_Done,
            eIndexerMessage_Aborted
        } EIndexerMessage;

        DnaMLX(sQPrideProc & qprideproc)
            : qp(qprideproc) /*, subjectIdxLockId(0), flagSet(sBioseqAlignment::fAlignForward), subbiomode(sBioseq::eBioModeShort), qrybiomode(sBioseq::eBioModeShort), separateHiveseqs(false), qryInFastQ(false), keepOriginalSubId(false),
              keepOriginalQryId(false), keepRefNs(true), scoreFilter(0), seedSize(28), evalueFilter(0), minMatchLength(75),
               maxMissQueryPercent(15), frmProduceRandomReadsForNT(false), Sub(qp.user), Qry(qp.user)*/
        {
        }
        virtual ~DnaMLX();

        /**
         * @brief Populate parameters within the object.
         * 
         * Reads the parameters from the HTML form.  Records a copy of this within the object for access later.
         * 
         * @param pForm The HTML form sent from the front end.  This object contains all of the entries as specified in the database.
         * @param user The user object.
         * @param objs Vector of user process objects.
         */
        virtual void readParams(sVar * pForm, sUsr * user, sVec<sUsrProc> & objs);

        /**
         * @brief Retrieves Query info 
         * 
         * Retrieves Query info from the HTML form
         * 
         * @param buf Buffer to populate.
         * @return const char* Pointer to populated buffer.
         */
        virtual const char * queryGet(sStr * buf = 0)
        {
            static sStr lbuf;
            return qp.formValue("query", buf ? buf : &lbuf);
        }

        /**
         * @brief Data Preparation
         * 
         * Prepare the data for ingestion into the model.
         * 
         * @param query 
         * @return EIndexerMessage 
         */
        virtual EIndexerMessage PepareData(const char * query);

        // virtual EIndexerMessage Align(const char * query);


        // virtual idx ParseAlignment(const idx keepAllMatches, sDic<idx> * unalignedList);

        //! Returns the extension of the output file for the aligner selected.
        /*! \returns Pointer to extension.
         */
        virtual const char * resultExtension(void) const = 0;

        virtual bool getPathsForFinalProcessing(sStr & paths00)
        {
            return false;
        }
        idx FinalProcessing();

    protected:

        friend class DnaMLXProc;

        sQPrideProc & qp;
        sDic<idx> idMap;
        idx subjectIdxLockId, flagSet;
        sBioseq::EBioMode subbiomode, qrybiomode;
        bool separateHiveseqs;
        sStr algorithm;
        bool qryInFastQ, keepOriginalSubId, keepOriginalQryId, keepRefNs;
        idx scoreFilter, seedSize;
        sStr subjectFile00;
        sStr additionalArguments, additionalCommandLineParameters;
        sStr resourceRoot, referenceAnnotationFile;
        real evalueFilter;
        idx minMatchLength, maxMissQueryPercent;
        bool frmProduceRandomReadsForNT;
        sHiveseq Sub, Qry;
        sDic<sHiveseq> QryList;

        const char * const getWorkDir(bool algo = false)
        {
            static sStr workDir;
            workDir.cut0cut(0);
            qp.cfgStr(&workDir, 0, "qm.tempDirectory");
            if( workDir ) {
                workDir.printf("%s-qry-%" DEC "/%s", qp.svc.name, qp.reqId, algo ? algorithm.ptr() : "");
            }
            return workDir;
        }
};


#endif // sDnaML_hpp

