﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2015 - TortoiseGit

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#include "stdafx.h"
#include "TortoiseGitBlameData.h"
#include "PathUtils.h"
#include "DirFileEnum.h"

class CTortoiseGitBlameDataWithTestRepoFixture : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		g_Git.m_CurrentDir = m_Dir.GetTempDir();
		CString resourcesDir = CPathUtils::GetAppDirectory() + _T("\\resources");
		if (!PathIsDirectory(resourcesDir))
		{
			resourcesDir = CPathUtils::GetAppDirectory() + _T("\\..\\..\\..\\test\\UnitTests\\resources");
			ASSERT_TRUE(PathIsDirectory(resourcesDir));
		}
		EXPECT_TRUE(CreateDirectory(m_Dir.GetTempDir() + _T("\\.git"), nullptr));
		CString repoDir = resourcesDir + _T("\\git-repo1");
		CDirFileEnum finder(repoDir);
		bool isDir;
		CString filepath;
		while (finder.NextFile(filepath, &isDir))
		{
			CString relpath = filepath.Mid(repoDir.GetLength());
			if (isDir)
				EXPECT_TRUE(CreateDirectory(m_Dir.GetTempDir() + _T("\\.git") + relpath, nullptr));
			else
				EXPECT_TRUE(CopyFile(filepath, m_Dir.GetTempDir() + _T("\\.git") + relpath, false));
		}
	}

	CAutoTempDir m_Dir;
};

TEST_F(CTortoiseGitBlameDataWithTestRepoFixture, ParseBlameOutput_ASCII)
{
	// git.exe blame -p 5617c7b4cb9466da214b3a01f5ffef561c51cead -- release.txt
	BYTE inputByteArray[] = {
		0x30, 0x33, 0x37, 0x66, 0x63, 0x33, 0x65, 0x35, 0x66, 0x36, 0x65, 0x39, 0x35, 0x66, 0x30, 0x31,
		0x32, 0x31, 0x65, 0x39, 0x35, 0x34, 0x63, 0x62, 0x33, 0x39, 0x32, 0x32, 0x64, 0x39, 0x34, 0x33,
		0x63, 0x39, 0x65, 0x34, 0x32, 0x61, 0x35, 0x34, 0x20, 0x31, 0x20, 0x31, 0x20, 0x37, 0x0A, 0x61,
		0x75, 0x74, 0x68, 0x6F, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63,
		0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D, 0x61, 0x69,
		0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65,
		0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65,
		0x20, 0x31, 0x33, 0x30, 0x35, 0x33, 0x37, 0x33, 0x37, 0x39, 0x38, 0x0A, 0x61, 0x75, 0x74, 0x68,
		0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x63, 0x6F, 0x6D, 0x6D,
		0x69, 0x74, 0x74, 0x65, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63,
		0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D,
		0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77,
		0x61, 0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65,
		0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x33, 0x30, 0x35, 0x33, 0x37, 0x33, 0x37, 0x39,
		0x38, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B,
		0x30, 0x32, 0x30, 0x30, 0x0A, 0x73, 0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x61, 0x64, 0x64,
		0x65, 0x64, 0x20, 0x6E, 0x6F, 0x74, 0x65, 0x73, 0x20, 0x61, 0x62, 0x6F, 0x75, 0x74, 0x20, 0x62,
		0x75, 0x6D, 0x70, 0x69, 0x6E, 0x67, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x6E,
		0x75, 0x6D, 0x62, 0x65, 0x72, 0x73, 0x0A, 0x66, 0x69, 0x6C, 0x65, 0x6E, 0x61, 0x6D, 0x65, 0x20,
		0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x09, 0x5C, 0x73, 0x65,
		0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x6E, 0x75,
		0x6D, 0x62, 0x65, 0x72, 0x73, 0x0D, 0x0A, 0x30, 0x33, 0x37, 0x66, 0x63, 0x33, 0x65, 0x35, 0x66,
		0x36, 0x65, 0x39, 0x35, 0x66, 0x30, 0x31, 0x32, 0x31, 0x65, 0x39, 0x35, 0x34, 0x63, 0x62, 0x33,
		0x39, 0x32, 0x32, 0x64, 0x39, 0x34, 0x33, 0x63, 0x39, 0x65, 0x34, 0x32, 0x61, 0x35, 0x34, 0x20,
		0x32, 0x20, 0x32, 0x0A, 0x09, 0x0D, 0x0A, 0x30, 0x33, 0x37, 0x66, 0x63, 0x33, 0x65, 0x35, 0x66,
		0x36, 0x65, 0x39, 0x35, 0x66, 0x30, 0x31, 0x32, 0x31, 0x65, 0x39, 0x35, 0x34, 0x63, 0x62, 0x33,
		0x39, 0x32, 0x32, 0x64, 0x39, 0x34, 0x33, 0x63, 0x39, 0x65, 0x34, 0x32, 0x61, 0x35, 0x34, 0x20,
		0x33, 0x20, 0x33, 0x0A, 0x09, 0x55, 0x70, 0x67, 0x72, 0x61, 0x64, 0x65, 0x20, 0x6E, 0x75, 0x6D,
		0x62, 0x65, 0x72, 0x73, 0x20, 0x69, 0x6E, 0x3A, 0x0D, 0x0A, 0x30, 0x33, 0x37, 0x66, 0x63, 0x33,
		0x65, 0x35, 0x66, 0x36, 0x65, 0x39, 0x35, 0x66, 0x30, 0x31, 0x32, 0x31, 0x65, 0x39, 0x35, 0x34,
		0x63, 0x62, 0x33, 0x39, 0x32, 0x32, 0x64, 0x39, 0x34, 0x33, 0x63, 0x39, 0x65, 0x34, 0x32, 0x61,
		0x35, 0x34, 0x20, 0x34, 0x20, 0x34, 0x0A, 0x09, 0x0D, 0x0A, 0x30, 0x33, 0x37, 0x66, 0x63, 0x33,
		0x65, 0x35, 0x66, 0x36, 0x65, 0x39, 0x35, 0x66, 0x30, 0x31, 0x32, 0x31, 0x65, 0x39, 0x35, 0x34,
		0x63, 0x62, 0x33, 0x39, 0x32, 0x32, 0x64, 0x39, 0x34, 0x33, 0x63, 0x39, 0x65, 0x34, 0x32, 0x61,
		0x35, 0x34, 0x20, 0x35, 0x20, 0x35, 0x0A, 0x09, 0x2D, 0x23, 0x20, 0x64, 0x6F, 0x63, 0x2F, 0x64,
		0x6F, 0x63, 0x2E, 0x62, 0x75, 0x69, 0x6C, 0x64, 0x2E, 0x69, 0x6E, 0x63, 0x6C, 0x75, 0x64, 0x65,
		0x0D, 0x0A, 0x30, 0x33, 0x37, 0x66, 0x63, 0x33, 0x65, 0x35, 0x66, 0x36, 0x65, 0x39, 0x35, 0x66,
		0x30, 0x31, 0x32, 0x31, 0x65, 0x39, 0x35, 0x34, 0x63, 0x62, 0x33, 0x39, 0x32, 0x32, 0x64, 0x39,
		0x34, 0x33, 0x63, 0x39, 0x65, 0x34, 0x32, 0x61, 0x35, 0x34, 0x20, 0x36, 0x20, 0x36, 0x0A, 0x09,
		0x2D, 0x23, 0x20, 0x73, 0x72, 0x63, 0x2F, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x2E, 0x68,
		0x0D, 0x0A, 0x30, 0x33, 0x37, 0x66, 0x63, 0x33, 0x65, 0x35, 0x66, 0x36, 0x65, 0x39, 0x35, 0x66,
		0x30, 0x31, 0x32, 0x31, 0x65, 0x39, 0x35, 0x34, 0x63, 0x62, 0x33, 0x39, 0x32, 0x32, 0x64, 0x39,
		0x34, 0x33, 0x63, 0x39, 0x65, 0x34, 0x32, 0x61, 0x35, 0x34, 0x20, 0x37, 0x20, 0x37, 0x0A, 0x09,
		0x2D, 0x23, 0x20, 0x73, 0x72, 0x63, 0x2F, 0x54, 0x6F, 0x72, 0x74, 0x6F, 0x69, 0x73, 0x65, 0x47,
		0x69, 0x74, 0x53, 0x65, 0x74, 0x75, 0x70, 0x2F, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x4E,
		0x75, 0x6D, 0x62, 0x65, 0x72, 0x49, 0x6E, 0x63, 0x6C, 0x75, 0x64, 0x65, 0x2E, 0x77, 0x78, 0x69,
		0x0D, 0x0A, 0x62, 0x32, 0x66, 0x62, 0x30, 0x64, 0x30, 0x64, 0x34, 0x38, 0x38, 0x34, 0x33, 0x34,
		0x63, 0x32, 0x32, 0x31, 0x34, 0x37, 0x32, 0x66, 0x62, 0x38, 0x39, 0x39, 0x36, 0x38, 0x33, 0x37,
		0x37, 0x35, 0x31, 0x30, 0x37, 0x33, 0x64, 0x66, 0x35, 0x32, 0x20, 0x38, 0x20, 0x38, 0x20, 0x31,
		0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72,
		0x69, 0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D,
		0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61,
		0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69,
		0x6D, 0x65, 0x20, 0x31, 0x33, 0x33, 0x39, 0x32, 0x34, 0x38, 0x39, 0x32, 0x30, 0x0A, 0x61, 0x75,
		0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x63, 0x6F,
		0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72,
		0x69, 0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65,
		0x72, 0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73,
		0x2D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74,
		0x74, 0x65, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x33, 0x33, 0x39, 0x32, 0x35, 0x30,
		0x32, 0x36, 0x38, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A,
		0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x73, 0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x73,
		0x77, 0x69, 0x74, 0x63, 0x68, 0x65, 0x64, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x72, 0x61, 0x73, 0x68,
		0x2D, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x63, 0x72, 0x61, 0x73, 0x68, 0x20, 0x68, 0x61,
		0x6E, 0x64, 0x6C, 0x65, 0x72, 0x0A, 0x70, 0x72, 0x65, 0x76, 0x69, 0x6F, 0x75, 0x73, 0x20, 0x37,
		0x61, 0x34, 0x35, 0x38, 0x63, 0x31, 0x63, 0x62, 0x64, 0x39, 0x33, 0x31, 0x62, 0x34, 0x36, 0x66,
		0x33, 0x30, 0x37, 0x65, 0x38, 0x34, 0x37, 0x63, 0x34, 0x33, 0x34, 0x33, 0x65, 0x62, 0x36, 0x36,
		0x35, 0x66, 0x32, 0x30, 0x36, 0x37, 0x63, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x2E,
		0x74, 0x78, 0x74, 0x0A, 0x66, 0x69, 0x6C, 0x65, 0x6E, 0x61, 0x6D, 0x65, 0x20, 0x72, 0x65, 0x6C,
		0x65, 0x61, 0x73, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x09, 0x0D, 0x0A, 0x62, 0x37, 0x62, 0x64,
		0x62, 0x38, 0x65, 0x35, 0x64, 0x64, 0x66, 0x32, 0x65, 0x62, 0x62, 0x39, 0x63, 0x36, 0x35, 0x32,
		0x35, 0x64, 0x35, 0x39, 0x63, 0x30, 0x37, 0x36, 0x65, 0x64, 0x61, 0x61, 0x32, 0x32, 0x39, 0x38,
		0x64, 0x66, 0x38, 0x36, 0x20, 0x39, 0x20, 0x39, 0x20, 0x31, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F,
		0x72, 0x20, 0x53, 0x75, 0x70, 0x20, 0x59, 0x75, 0x74, 0x20, 0x53, 0x75, 0x6D, 0x0A, 0x61, 0x75,
		0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x63, 0x68, 0x33, 0x63, 0x6F,
		0x6F, 0x6C, 0x69, 0x40, 0x67, 0x6D, 0x61, 0x69, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x3E, 0x0A, 0x61,
		0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x34, 0x30, 0x38, 0x31,
		0x38, 0x32, 0x39, 0x30, 0x38, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20,
		0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x20,
		0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A,
		0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C,
		0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x64, 0x65,
		0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65,
		0x20, 0x31, 0x34, 0x30, 0x38, 0x36, 0x33, 0x36, 0x39, 0x35, 0x36, 0x0A, 0x63, 0x6F, 0x6D, 0x6D,
		0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x73,
		0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x52, 0x65, 0x70, 0x6C, 0x61, 0x63, 0x65, 0x20, 0x63,
		0x72, 0x61, 0x73, 0x68, 0x2D, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x2E, 0x63, 0x6F, 0x6D, 0x20,
		0x77, 0x69, 0x74, 0x68, 0x20, 0x64, 0x72, 0x64, 0x75, 0x6D, 0x70, 0x2E, 0x63, 0x6F, 0x6D, 0x20,
		0x73, 0x69, 0x6E, 0x63, 0x65, 0x20, 0x74, 0x68, 0x61, 0x74, 0x27, 0x73, 0x20, 0x74, 0x68, 0x65,
		0x20, 0x6E, 0x65, 0x77, 0x20, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x0A, 0x70, 0x72, 0x65,
		0x76, 0x69, 0x6F, 0x75, 0x73, 0x20, 0x39, 0x31, 0x30, 0x62, 0x33, 0x65, 0x62, 0x30, 0x63, 0x33,
		0x33, 0x39, 0x65, 0x65, 0x32, 0x37, 0x64, 0x38, 0x36, 0x39, 0x65, 0x38, 0x32, 0x39, 0x30, 0x66,
		0x35, 0x66, 0x63, 0x66, 0x61, 0x30, 0x35, 0x62, 0x31, 0x33, 0x32, 0x33, 0x62, 0x33, 0x20, 0x72,
		0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x66, 0x69, 0x6C, 0x65, 0x6E,
		0x61, 0x6D, 0x65, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x0A,
		0x09, 0x55, 0x70, 0x6C, 0x6F, 0x61, 0x64, 0x20, 0x73, 0x79, 0x6D, 0x62, 0x6F, 0x6C, 0x73, 0x20,
		0x74, 0x6F, 0x20, 0x64, 0x72, 0x64, 0x75, 0x6D, 0x70, 0x2E, 0x63, 0x6F, 0x6D, 0x0D, 0x0A, 0x64,
		0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38, 0x35, 0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64,
		0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38, 0x36, 0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34,
		0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20, 0x31, 0x30, 0x20, 0x31, 0x30, 0x20, 0x31, 0x0A,
		0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69,
		0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D, 0x61,
		0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72,
		0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69, 0x6D,
		0x65, 0x20, 0x31, 0x33, 0x35, 0x36, 0x37, 0x32, 0x36, 0x38, 0x36, 0x39, 0x0A, 0x61, 0x75, 0x74,
		0x68, 0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x31, 0x30, 0x30, 0x0A, 0x63, 0x6F, 0x6D,
		0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69,
		0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72,
		0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D,
		0x77, 0x61, 0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74,
		0x65, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x33, 0x35, 0x36, 0x37, 0x32, 0x38, 0x32,
		0x38, 0x31, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A, 0x20,
		0x2B, 0x30, 0x31, 0x30, 0x30, 0x0A, 0x73, 0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x55, 0x70,
		0x64, 0x61, 0x74, 0x65, 0x64, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x20, 0x70, 0x72,
		0x6F, 0x63, 0x65, 0x73, 0x73, 0x20, 0x69, 0x6E, 0x66, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x69, 0x6F,
		0x6E, 0x0A, 0x70, 0x72, 0x65, 0x76, 0x69, 0x6F, 0x75, 0x73, 0x20, 0x31, 0x32, 0x33, 0x62, 0x32,
		0x35, 0x35, 0x62, 0x65, 0x64, 0x62, 0x65, 0x30, 0x63, 0x30, 0x61, 0x37, 0x38, 0x32, 0x32, 0x64,
		0x61, 0x37, 0x39, 0x35, 0x31, 0x65, 0x65, 0x65, 0x61, 0x38, 0x64, 0x32, 0x61, 0x65, 0x63, 0x38,
		0x38, 0x36, 0x31, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x0A,
		0x66, 0x69, 0x6C, 0x65, 0x6E, 0x61, 0x6D, 0x65, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65,
		0x2E, 0x74, 0x78, 0x74, 0x0A, 0x09, 0x0D, 0x0A, 0x32, 0x63, 0x63, 0x31, 0x63, 0x66, 0x31, 0x32,
		0x64, 0x36, 0x62, 0x38, 0x64, 0x64, 0x33, 0x62, 0x33, 0x62, 0x32, 0x32, 0x38, 0x66, 0x30, 0x37,
		0x34, 0x36, 0x32, 0x61, 0x34, 0x31, 0x33, 0x31, 0x36, 0x35, 0x35, 0x34, 0x61, 0x36, 0x30, 0x30,
		0x20, 0x31, 0x31, 0x20, 0x31, 0x31, 0x20, 0x31, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x20,
		0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A,
		0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61,
		0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x61,
		0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x33, 0x39, 0x39, 0x38,
		0x30, 0x34, 0x36, 0x35, 0x38, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20,
		0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x20,
		0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A,
		0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C,
		0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x64, 0x65,
		0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65,
		0x20, 0x31, 0x33, 0x39, 0x39, 0x38, 0x30, 0x34, 0x36, 0x35, 0x38, 0x0A, 0x63, 0x6F, 0x6D, 0x6D,
		0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x73,
		0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x47, 0x6F, 0x6F, 0x67, 0x6C, 0x65, 0x43, 0x6F, 0x64,
		0x65, 0x20, 0x64, 0x6F, 0x65, 0x73, 0x6E, 0x27, 0x74, 0x20, 0x70, 0x72, 0x6F, 0x76, 0x69, 0x64,
		0x65, 0x20, 0x64, 0x6F, 0x77, 0x6E, 0x6C, 0x6F, 0x61, 0x64, 0x20, 0x73, 0x65, 0x72, 0x76, 0x69,
		0x63, 0x65, 0x73, 0x20, 0x61, 0x6E, 0x79, 0x20, 0x6D, 0x6F, 0x72, 0x65, 0x0A, 0x70, 0x72, 0x65,
		0x76, 0x69, 0x6F, 0x75, 0x73, 0x20, 0x61, 0x32, 0x66, 0x38, 0x30, 0x65, 0x39, 0x35, 0x36, 0x39,
		0x34, 0x32, 0x62, 0x37, 0x39, 0x36, 0x31, 0x35, 0x33, 0x39, 0x34, 0x62, 0x63, 0x37, 0x31, 0x63,
		0x33, 0x38, 0x32, 0x34, 0x61, 0x34, 0x37, 0x39, 0x65, 0x38, 0x63, 0x30, 0x38, 0x30, 0x20, 0x72,
		0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x66, 0x69, 0x6C, 0x65, 0x6E,
		0x61, 0x6D, 0x65, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x2E, 0x74, 0x78, 0x74, 0x0A,
		0x09, 0x55, 0x70, 0x6C, 0x6F, 0x61, 0x64, 0x20, 0x28, 0x73, 0x69, 0x67, 0x6E, 0x65, 0x64, 0x29,
		0x20, 0x6D, 0x73, 0x69, 0x2D, 0x66, 0x69, 0x6C, 0x65, 0x73, 0x20, 0x28, 0x64, 0x6F, 0x20, 0x6E,
		0x6F, 0x74, 0x20, 0x66, 0x6F, 0x72, 0x67, 0x65, 0x74, 0x20, 0x74, 0x6F, 0x20, 0x75, 0x70, 0x6C,
		0x6F, 0x61, 0x64, 0x20, 0x64, 0x65, 0x62, 0x75, 0x67, 0x20, 0x73, 0x79, 0x6D, 0x62, 0x6F, 0x6C,
		0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x6C, 0x61, 0x6E, 0x67, 0x75, 0x61, 0x67, 0x65, 0x20, 0x70,
		0x61, 0x63, 0x6B, 0x73, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x73, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x20,
		0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x73, 0x29, 0x2E, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63,
		0x34, 0x33, 0x35, 0x62, 0x38, 0x35, 0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62,
		0x63, 0x34, 0x64, 0x63, 0x38, 0x36, 0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35,
		0x34, 0x62, 0x63, 0x62, 0x20, 0x31, 0x32, 0x20, 0x31, 0x32, 0x20, 0x31, 0x0A, 0x09, 0x4E, 0x61,
		0x6D, 0x69, 0x6E, 0x67, 0x2D, 0x63, 0x6F, 0x6E, 0x76, 0x65, 0x6E, 0x74, 0x69, 0x6F, 0x6E, 0x3A,
		0x0D, 0x0A, 0x32, 0x63, 0x63, 0x31, 0x63, 0x66, 0x31, 0x32, 0x64, 0x36, 0x62, 0x38, 0x64, 0x64,
		0x33, 0x62, 0x33, 0x62, 0x32, 0x32, 0x38, 0x66, 0x30, 0x37, 0x34, 0x36, 0x32, 0x61, 0x34, 0x31,
		0x33, 0x31, 0x36, 0x35, 0x35, 0x34, 0x61, 0x36, 0x30, 0x30, 0x20, 0x31, 0x33, 0x20, 0x31, 0x33,
		0x20, 0x31, 0x0A, 0x09, 0x2D, 0x20, 0x73, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x20, 0x72, 0x65, 0x6C,
		0x65, 0x61, 0x73, 0x65, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38, 0x35,
		0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38, 0x36,
		0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20, 0x31,
		0x34, 0x20, 0x31, 0x34, 0x20, 0x32, 0x0A, 0x09, 0x20, 0x20, 0x2D, 0x20, 0x54, 0x6F, 0x72, 0x74,
		0x6F, 0x69, 0x73, 0x65, 0x47, 0x69, 0x74, 0x2D, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x2D,
		0x58, 0x58, 0x62, 0x69, 0x74, 0x2E, 0x6D, 0x73, 0x69, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63, 0x34,
		0x33, 0x35, 0x62, 0x38, 0x35, 0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62, 0x63,
		0x34, 0x64, 0x63, 0x38, 0x36, 0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35, 0x34,
		0x62, 0x63, 0x62, 0x20, 0x31, 0x35, 0x20, 0x31, 0x35, 0x0A, 0x09, 0x20, 0x20, 0x2D, 0x20, 0x54,
		0x6F, 0x72, 0x74, 0x6F, 0x69, 0x73, 0x65, 0x47, 0x69, 0x74, 0x2D, 0x4C, 0x61, 0x6E, 0x67, 0x75,
		0x61, 0x67, 0x65, 0x50, 0x61, 0x63, 0x6B, 0x2D, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x2D,
		0x58, 0x58, 0x62, 0x69, 0x74, 0x2D, 0x49, 0x53, 0x4F, 0x4C, 0x41, 0x4E, 0x47, 0x2E, 0x6D, 0x73,
		0x69, 0x0D, 0x0A, 0x32, 0x63, 0x63, 0x31, 0x63, 0x66, 0x31, 0x32, 0x64, 0x36, 0x62, 0x38, 0x64,
		0x64, 0x33, 0x62, 0x33, 0x62, 0x32, 0x32, 0x38, 0x66, 0x30, 0x37, 0x34, 0x36, 0x32, 0x61, 0x34,
		0x31, 0x33, 0x31, 0x36, 0x35, 0x35, 0x34, 0x61, 0x36, 0x30, 0x30, 0x20, 0x31, 0x36, 0x20, 0x31,
		0x36, 0x20, 0x33, 0x0A, 0x09, 0x20, 0x20, 0x2D, 0x20, 0x54, 0x6F, 0x72, 0x74, 0x6F, 0x69, 0x73,
		0x65, 0x47, 0x69, 0x74, 0x2D, 0x44, 0x65, 0x62, 0x75, 0x67, 0x53, 0x79, 0x6D, 0x62, 0x6F, 0x6C,
		0x73, 0x2D, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E, 0x2E, 0x37, 0x7A, 0x0D, 0x0A, 0x32, 0x63,
		0x63, 0x31, 0x63, 0x66, 0x31, 0x32, 0x64, 0x36, 0x62, 0x38, 0x64, 0x64, 0x33, 0x62, 0x33, 0x62,
		0x32, 0x32, 0x38, 0x66, 0x30, 0x37, 0x34, 0x36, 0x32, 0x61, 0x34, 0x31, 0x33, 0x31, 0x36, 0x35,
		0x35, 0x34, 0x61, 0x36, 0x30, 0x30, 0x20, 0x31, 0x37, 0x20, 0x31, 0x37, 0x0A, 0x09, 0x2D, 0x20,
		0x70, 0x72, 0x65, 0x76, 0x69, 0x65, 0x77, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x0D,
		0x0A, 0x32, 0x63, 0x63, 0x31, 0x63, 0x66, 0x31, 0x32, 0x64, 0x36, 0x62, 0x38, 0x64, 0x64, 0x33,
		0x62, 0x33, 0x62, 0x32, 0x32, 0x38, 0x66, 0x30, 0x37, 0x34, 0x36, 0x32, 0x61, 0x34, 0x31, 0x33,
		0x31, 0x36, 0x35, 0x35, 0x34, 0x61, 0x36, 0x30, 0x30, 0x20, 0x31, 0x38, 0x20, 0x31, 0x38, 0x0A,
		0x09, 0x20, 0x20, 0x2D, 0x20, 0x54, 0x6F, 0x72, 0x74, 0x6F, 0x69, 0x73, 0x65, 0x47, 0x69, 0x74,
		0x2D, 0x70, 0x72, 0x65, 0x76, 0x69, 0x65, 0x77, 0x2D, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4F, 0x4E,
		0x2D, 0x59, 0x59, 0x59, 0x59, 0x4D, 0x4D, 0x44, 0x44, 0x2D, 0x28, 0x53, 0x48, 0x41, 0x31, 0x2D,
		0x37, 0x63, 0x68, 0x61, 0x72, 0x73, 0x29, 0x2D, 0x58, 0x58, 0x62, 0x69, 0x74, 0x0D, 0x0A, 0x64,
		0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38, 0x35, 0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64,
		0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38, 0x36, 0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34,
		0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20, 0x31, 0x39, 0x20, 0x31, 0x39, 0x20, 0x33, 0x0A,
		0x09, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38, 0x35, 0x62, 0x39, 0x39,
		0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38, 0x36, 0x31, 0x38, 0x32,
		0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20, 0x32, 0x30, 0x20, 0x32,
		0x30, 0x0A, 0x09, 0x47, 0x6F, 0x6F, 0x67, 0x6C, 0x65, 0x20, 0x63, 0x6F, 0x64, 0x65, 0x20, 0x74,
		0x61, 0x73, 0x6B, 0x73, 0x3A, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38,
		0x35, 0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38,
		0x36, 0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20,
		0x32, 0x31, 0x20, 0x32, 0x31, 0x0A, 0x09, 0x2D, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x61, 0x20, 0x73,
		0x74, 0x61, 0x62, 0x6C, 0x65, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x0D, 0x0A, 0x32,
		0x63, 0x63, 0x31, 0x63, 0x66, 0x31, 0x32, 0x64, 0x36, 0x62, 0x38, 0x64, 0x64, 0x33, 0x62, 0x33,
		0x62, 0x32, 0x32, 0x38, 0x66, 0x30, 0x37, 0x34, 0x36, 0x32, 0x61, 0x34, 0x31, 0x33, 0x31, 0x36,
		0x35, 0x35, 0x34, 0x61, 0x36, 0x30, 0x30, 0x20, 0x32, 0x32, 0x20, 0x32, 0x32, 0x20, 0x31, 0x0A,
		0x09, 0x20, 0x20, 0x2D, 0x20, 0x75, 0x70, 0x64, 0x61, 0x74, 0x65, 0x20, 0x66, 0x72, 0x6F, 0x6E,
		0x74, 0x2F, 0x73, 0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x70, 0x61, 0x67, 0x65, 0x20, 0x61,
		0x73, 0x20, 0x77, 0x65, 0x6C, 0x6C, 0x20, 0x61, 0x73, 0x20, 0x22, 0x44, 0x6F, 0x77, 0x6E, 0x6C,
		0x6F, 0x61, 0x64, 0x73, 0x22, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x22, 0x52, 0x65, 0x6C, 0x65, 0x61,
		0x73, 0x65, 0x4E, 0x6F, 0x74, 0x65, 0x73, 0x22, 0x20, 0x28, 0x6D, 0x61, 0x6B, 0x65, 0x20, 0x73,
		0x75, 0x72, 0x65, 0x20, 0x69, 0x74, 0x20, 0x63, 0x6F, 0x6E, 0x74, 0x61, 0x69, 0x6E, 0x73, 0x20,
		0x6E, 0x6F, 0x20, 0x75, 0x6E, 0x77, 0x61, 0x6E, 0x74, 0x65, 0x64, 0x20, 0x6C, 0x69, 0x6E, 0x6B,
		0x73, 0x29, 0x20, 0x77, 0x69, 0x6B, 0x69, 0x20, 0x70, 0x61, 0x67, 0x65, 0x73, 0x0D, 0x0A, 0x64,
		0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38, 0x35, 0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64,
		0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38, 0x36, 0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34,
		0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20, 0x33, 0x33, 0x20, 0x32, 0x33, 0x20, 0x34, 0x0A,
		0x09, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38, 0x35, 0x62, 0x39, 0x39,
		0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38, 0x36, 0x31, 0x38, 0x32,
		0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20, 0x33, 0x34, 0x20, 0x32,
		0x34, 0x0A, 0x09, 0x55, 0x70, 0x64, 0x61, 0x74, 0x65, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F,
		0x6E, 0x2E, 0x74, 0x78, 0x74, 0x20, 0x28, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x63, 0x6F,
		0x64, 0x65, 0x2E, 0x67, 0x6F, 0x6F, 0x67, 0x6C, 0x65, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x70, 0x2F,
		0x74, 0x6F, 0x72, 0x74, 0x6F, 0x69, 0x73, 0x65, 0x67, 0x69, 0x74, 0x2F, 0x73, 0x6F, 0x75, 0x72,
		0x63, 0x65, 0x2F, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x6F, 0x75, 0x74, 0x3F, 0x72, 0x65, 0x70, 0x6F,
		0x3D, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x29, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x61, 0x75,
		0x74, 0x6F, 0x20, 0x75, 0x70, 0x64, 0x61, 0x74, 0x65, 0x72, 0x20, 0x28, 0x66, 0x6F, 0x72, 0x20,
		0x61, 0x20, 0x73, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65,
		0x20, 0x6D, 0x61, 0x6B, 0x65, 0x20, 0x73, 0x75, 0x72, 0x65, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69,
		0x6F, 0x6E, 0x2D, 0x70, 0x72, 0x65, 0x76, 0x69, 0x65, 0x77, 0x2E, 0x74, 0x78, 0x74, 0x20, 0x68,
		0x61, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x73, 0x61, 0x6D, 0x65, 0x20, 0x63, 0x6F, 0x6E, 0x74,
		0x65, 0x6E, 0x74, 0x20, 0x61, 0x73, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x2E, 0x74,
		0x78, 0x74, 0x29, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38, 0x35, 0x62,
		0x39, 0x39, 0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38, 0x36, 0x31,
		0x38, 0x32, 0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20, 0x33, 0x35,
		0x20, 0x32, 0x35, 0x0A, 0x09, 0x0D, 0x0A, 0x64, 0x39, 0x39, 0x63, 0x34, 0x33, 0x35, 0x62, 0x38,
		0x35, 0x62, 0x39, 0x39, 0x63, 0x39, 0x63, 0x64, 0x62, 0x61, 0x62, 0x63, 0x34, 0x64, 0x63, 0x38,
		0x36, 0x31, 0x38, 0x32, 0x39, 0x62, 0x36, 0x34, 0x31, 0x31, 0x35, 0x34, 0x62, 0x63, 0x62, 0x20,
		0x33, 0x36, 0x20, 0x32, 0x36, 0x0A, 0x09, 0x53, 0x65, 0x6E, 0x64, 0x20, 0x61, 0x6E, 0x6E, 0x6F,
		0x75, 0x63, 0x65, 0x6D, 0x65, 0x6E, 0x74, 0x20, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x28, 0x66, 0x6F,
		0x72, 0x20, 0x73, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65,
		0x73, 0x29, 0x0D, 0x0A
	};

	BYTE_VECTOR blame_output;
	EXPECT_EQ(0, blame_output.append(inputByteArray, sizeof(inputByteArray)));
	EXPECT_EQ(sizeof(inputByteArray), blame_output.size());

	CTortoiseGitBlameData data;
	CGitHashMap hashToRev;
	data.ParseBlameOutput(blame_output, hashToRev, 0, false, true);

	EXPECT_EQ(26, data.GetNumberOfLines());

	for (int i = 0; i < 26; ++i)
		EXPECT_TRUE(data.IsValidLine(i));
	EXPECT_FALSE(data.IsValidLine(26));

	for (int i = 0; i < 26; ++i)
	{
		if (i != 8)
			EXPECT_STREQ(L"Sven Strickroth", data.GetAuthor(i));
		else
			EXPECT_STREQ(L"Sup Yut Sum", data.GetAuthor(i));
	}

	const CString hashes[] = { L"037fc3e5", L"037fc3e5", L"037fc3e5", L"037fc3e5", L"037fc3e5", L"037fc3e5", L"037fc3e5", L"b2fb0d0d", L"b7bdb8e5", L"d99c435b", L"2cc1cf12", L"d99c435b", L"2cc1cf12", L"d99c435b", L"d99c435b", L"2cc1cf12", L"2cc1cf12", L"2cc1cf12", L"d99c435b", L"d99c435b", L"d99c435b", L"2cc1cf12", L"d99c435b", L"d99c435b", L"d99c435b", L"d99c435b" };
	for (int i = 0; i < 26; ++i)
		EXPECT_STREQ(hashes[i], data.GetHash(i).ToString().Left(8));

	EXPECT_STREQ(L"2011-05-14 11:49:58", data.GetDate(0));
	EXPECT_STREQ(L"2011-05-14 11:49:58", data.GetDate(6));
	EXPECT_STREQ(L"2012-12-28 20:34:29", data.GetDate(13));
	EXPECT_STREQ(L"2012-12-28 20:34:29", data.GetDate(25));

	data.UpdateEncoding(CP_UTF8);
	const CStringA lines[] = { "\\section Version numbers", "", "Upgrade numbers in:", "", "-# doc/doc.build.include", "-# src/version.h", "-# src/TortoiseGitSetup/VersionNumberInclude.wxi", "", "Upload symbols to drdump.com", "", "Upload (signed) msi-files (do not forget to upload debug symbols and language packs for stable releases).", "Naming-convention:", "- stable release", "  - TortoiseGit-VERSION-XXbit.msi", "  - TortoiseGit-LanguagePack-VERSION-XXbit-ISOLANG.msi", "  - TortoiseGit-DebugSymbols-VERSION.7z", "- preview release", "  - TortoiseGit-preview-VERSION-YYYYMMDD-(SHA1-7chars)-XXbit", "", "Google code tasks:", "- for a stable release", "  - update front/summary page as well as \"Downloads\" and \"ReleaseNotes\" (make sure it contains no unwanted links) wiki pages", "", "Update version.txt (http://code.google.com/p/tortoisegit/source/checkout?repo=version) for auto updater (for a stable release make sure version-preview.txt has the same content as version.txt)", "", "Send annoucement mail (for stable releases)" };
	for (int i = 0; i < 26; ++i)
		EXPECT_STREQ(lines[i] + _T('\r'), data.GetUtf8Line(i));

	const int lineNumbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 33, 34, 35, 36 };
	for (int i = 0; i < 26; ++i)
		EXPECT_EQ(lineNumbers[i], data.GetOriginalLineNumber(i));

	for (int i = 0; i < 26; ++i)
		EXPECT_STREQ(L"release.txt", data.GetFilename(i));
}

TEST_F(CTortoiseGitBlameDataWithTestRepoFixture, ParseBlameOutput_UTF8_BOM)
{
	// git.exe blame -p 5617c7b4cb9466da214b3a01f5ffef561c51cead -- release.txt
	BYTE inputByteArray[] = {
		0x33, 0x64, 0x33, 0x34, 0x32, 0x30, 0x62, 0x62, 0x33, 0x66, 0x62, 0x36, 0x38, 0x61, 0x63, 0x30,
		0x66, 0x35, 0x65, 0x39, 0x31, 0x33, 0x36, 0x32, 0x38, 0x64, 0x35, 0x33, 0x37, 0x35, 0x33, 0x63,
		0x34, 0x39, 0x36, 0x34, 0x64, 0x62, 0x66, 0x66, 0x20, 0x31, 0x20, 0x31, 0x20, 0x31, 0x0A, 0x61,
		0x75, 0x74, 0x68, 0x6F, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63,
		0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D, 0x61, 0x69,
		0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65,
		0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65,
		0x20, 0x31, 0x33, 0x38, 0x32, 0x37, 0x39, 0x39, 0x37, 0x34, 0x35, 0x0A, 0x61, 0x75, 0x74, 0x68,
		0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x63, 0x6F, 0x6D, 0x6D,
		0x69, 0x74, 0x74, 0x65, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63,
		0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D,
		0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77,
		0x61, 0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65,
		0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x33, 0x38, 0x32, 0x37, 0x39, 0x39, 0x37, 0x34,
		0x35, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B,
		0x30, 0x32, 0x30, 0x30, 0x0A, 0x73, 0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x67, 0x67, 0x0A,
		0x70, 0x72, 0x65, 0x76, 0x69, 0x6F, 0x75, 0x73, 0x20, 0x36, 0x66, 0x63, 0x66, 0x36, 0x64, 0x37,
		0x30, 0x38, 0x32, 0x39, 0x38, 0x64, 0x66, 0x65, 0x62, 0x63, 0x30, 0x65, 0x61, 0x34, 0x61, 0x65,
		0x61, 0x33, 0x65, 0x63, 0x62, 0x32, 0x32, 0x66, 0x32, 0x66, 0x31, 0x31, 0x37, 0x38, 0x64, 0x30,
		0x66, 0x20, 0x75, 0x74, 0x66, 0x2D, 0x38, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x66, 0x69, 0x6C, 0x65,
		0x6E, 0x61, 0x6D, 0x65, 0x20, 0x75, 0x74, 0x66, 0x2D, 0x38, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x09,
		0xEF, 0xBB, 0xBF, 0xC3, 0xA4, 0x23, 0xC3, 0xA4, 0x66, 0x33, 0x34, 0xC3, 0xB6, 0xC3, 0xB6, 0xC3,
		0xB6, 0xC3, 0xA4, 0xC3, 0x9F, 0xE2, 0x82, 0xAC, 0x39, 0x38, 0x37, 0x35, 0x6F, 0x65, 0x0D, 0x0A,
		0x64, 0x37, 0x37, 0x32, 0x62, 0x31, 0x38, 0x39, 0x66, 0x31, 0x36, 0x34, 0x35, 0x39, 0x35, 0x65,
		0x32, 0x39, 0x65, 0x61, 0x38, 0x62, 0x63, 0x30, 0x32, 0x36, 0x37, 0x36, 0x62, 0x66, 0x64, 0x66,
		0x33, 0x63, 0x33, 0x30, 0x64, 0x36, 0x61, 0x38, 0x20, 0x32, 0x20, 0x32, 0x20, 0x33, 0x0A, 0x61,
		0x75, 0x74, 0x68, 0x6F, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63,
		0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D, 0x61, 0x69,
		0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65,
		0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65,
		0x20, 0x31, 0x33, 0x38, 0x32, 0x37, 0x39, 0x39, 0x35, 0x39, 0x30, 0x0A, 0x61, 0x75, 0x74, 0x68,
		0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x63, 0x6F, 0x6D, 0x6D,
		0x69, 0x74, 0x74, 0x65, 0x72, 0x20, 0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63,
		0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D,
		0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77,
		0x61, 0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65,
		0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x33, 0x38, 0x32, 0x37, 0x39, 0x39, 0x35, 0x39,
		0x30, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B,
		0x30, 0x32, 0x30, 0x30, 0x0A, 0x73, 0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x64, 0x73, 0x66,
		0x64, 0x73, 0x66, 0x0A, 0x62, 0x6F, 0x75, 0x6E, 0x64, 0x61, 0x72, 0x79, 0x0A, 0x66, 0x69, 0x6C,
		0x65, 0x6E, 0x61, 0x6D, 0x65, 0x20, 0x75, 0x74, 0x66, 0x2D, 0x38, 0x2E, 0x74, 0x78, 0x74, 0x0A,
		0x09, 0x66, 0x67, 0x64, 0x6A, 0x6B, 0x67, 0x6C, 0x73, 0x66, 0x64, 0x67, 0x0D, 0x0A, 0x64, 0x37,
		0x37, 0x32, 0x62, 0x31, 0x38, 0x39, 0x66, 0x31, 0x36, 0x34, 0x35, 0x39, 0x35, 0x65, 0x32, 0x39,
		0x65, 0x61, 0x38, 0x62, 0x63, 0x30, 0x32, 0x36, 0x37, 0x36, 0x62, 0x66, 0x64, 0x66, 0x33, 0x63,
		0x33, 0x30, 0x64, 0x36, 0x61, 0x38, 0x20, 0x33, 0x20, 0x33, 0x0A, 0x09, 0xC3, 0xB6, 0xC3, 0xA4,
		0xC3, 0xB6, 0xC3, 0xBC, 0x34, 0x35, 0x67, 0x0D, 0x0A, 0x64, 0x37, 0x37, 0x32, 0x62, 0x31, 0x38,
		0x39, 0x66, 0x31, 0x36, 0x34, 0x35, 0x39, 0x35, 0x65, 0x32, 0x39, 0x65, 0x61, 0x38, 0x62, 0x63,
		0x30, 0x32, 0x36, 0x37, 0x36, 0x62, 0x66, 0x64, 0x66, 0x33, 0x63, 0x33, 0x30, 0x64, 0x36, 0x61,
		0x38, 0x20, 0x34, 0x20, 0x34, 0x0A, 0x09, 0x66, 0x64, 0x67, 0x69, 0x26, 0xC2, 0xA7, 0x24, 0x25,
		0x26, 0x68, 0x66, 0x64, 0x73, 0x67, 0xC3, 0xA4, 0x0D, 0x0A, 0x36, 0x66, 0x63, 0x66, 0x36, 0x64,
		0x37, 0x30, 0x38, 0x32, 0x39, 0x38, 0x64, 0x66, 0x65, 0x62, 0x63, 0x30, 0x65, 0x61, 0x34, 0x61,
		0x65, 0x61, 0x33, 0x65, 0x63, 0x62, 0x32, 0x32, 0x66, 0x32, 0x66, 0x31, 0x31, 0x37, 0x38, 0x64,
		0x30, 0x66, 0x20, 0x35, 0x20, 0x35, 0x20, 0x31, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x20,
		0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A,
		0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C, 0x65, 0x6D, 0x61,
		0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x64, 0x65, 0x3E, 0x0A, 0x61,
		0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x31, 0x33, 0x38, 0x32, 0x37,
		0x39, 0x39, 0x36, 0x32, 0x33, 0x0A, 0x61, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x2D, 0x74, 0x7A, 0x20,
		0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x20,
		0x53, 0x76, 0x65, 0x6E, 0x20, 0x53, 0x74, 0x72, 0x69, 0x63, 0x6B, 0x72, 0x6F, 0x74, 0x68, 0x0A,
		0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x6D, 0x61, 0x69, 0x6C, 0x20, 0x3C,
		0x65, 0x6D, 0x61, 0x69, 0x6C, 0x40, 0x63, 0x73, 0x2D, 0x77, 0x61, 0x72, 0x65, 0x2E, 0x64, 0x65,
		0x3E, 0x0A, 0x63, 0x6F, 0x6D, 0x6D, 0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x69, 0x6D, 0x65,
		0x20, 0x31, 0x33, 0x38, 0x32, 0x37, 0x39, 0x39, 0x36, 0x32, 0x33, 0x0A, 0x63, 0x6F, 0x6D, 0x6D,
		0x69, 0x74, 0x74, 0x65, 0x72, 0x2D, 0x74, 0x7A, 0x20, 0x2B, 0x30, 0x32, 0x30, 0x30, 0x0A, 0x73,
		0x75, 0x6D, 0x6D, 0x61, 0x72, 0x79, 0x20, 0x66, 0x66, 0x64, 0x73, 0x66, 0x0A, 0x70, 0x72, 0x65,
		0x76, 0x69, 0x6F, 0x75, 0x73, 0x20, 0x64, 0x37, 0x37, 0x32, 0x62, 0x31, 0x38, 0x39, 0x66, 0x31,
		0x36, 0x34, 0x35, 0x39, 0x35, 0x65, 0x32, 0x39, 0x65, 0x61, 0x38, 0x62, 0x63, 0x30, 0x32, 0x36,
		0x37, 0x36, 0x62, 0x66, 0x64, 0x66, 0x33, 0x63, 0x33, 0x30, 0x64, 0x36, 0x61, 0x38, 0x20, 0x75,
		0x74, 0x66, 0x2D, 0x38, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x66, 0x69, 0x6C, 0x65, 0x6E, 0x61, 0x6D,
		0x65, 0x20, 0x75, 0x74, 0x66, 0x2D, 0x38, 0x2E, 0x74, 0x78, 0x74, 0x0A, 0x09, 0xC3, 0xA4, 0x23,
		0xC3, 0xA4, 0x66, 0x33, 0x34, 0xC3, 0xB6, 0xC3, 0xB6, 0xC3, 0xB6, 0xC3, 0xA4, 0xC3, 0x9F, 0xE2,
		0x82, 0xAC, 0x39, 0x38, 0x37, 0x35, 0x6F, 0x65, 0x0D, 0x0A, 0x64, 0x37, 0x37, 0x32, 0x62, 0x31,
		0x38, 0x39, 0x66, 0x31, 0x36, 0x34, 0x35, 0x39, 0x35, 0x65, 0x32, 0x39, 0x65, 0x61, 0x38, 0x62,
		0x63, 0x30, 0x32, 0x36, 0x37, 0x36, 0x62, 0x66, 0x64, 0x66, 0x33, 0x63, 0x33, 0x30, 0x64, 0x36,
		0x61, 0x38, 0x20, 0x36, 0x20, 0x36, 0x20, 0x34, 0x0A, 0x09, 0xC3, 0xB6, 0xC3, 0xA4, 0xC3, 0xBC,
		0x70, 0x66, 0x67, 0x6D, 0x66, 0x64, 0x67, 0x0D, 0x0A, 0x64, 0x37, 0x37, 0x32, 0x62, 0x31, 0x38,
		0x39, 0x66, 0x31, 0x36, 0x34, 0x35, 0x39, 0x35, 0x65, 0x32, 0x39, 0x65, 0x61, 0x38, 0x62, 0x63,
		0x30, 0x32, 0x36, 0x37, 0x36, 0x62, 0x66, 0x64, 0x66, 0x33, 0x63, 0x33, 0x30, 0x64, 0x36, 0x61,
		0x38, 0x20, 0x37, 0x20, 0x37, 0x0A, 0x09, 0xE2, 0x82, 0xAC, 0x66, 0x67, 0x66, 0x64, 0x73, 0x67,
		0x0D, 0x0A, 0x64, 0x37, 0x37, 0x32, 0x62, 0x31, 0x38, 0x39, 0x66, 0x31, 0x36, 0x34, 0x35, 0x39,
		0x35, 0x65, 0x32, 0x39, 0x65, 0x61, 0x38, 0x62, 0x63, 0x30, 0x32, 0x36, 0x37, 0x36, 0x62, 0x66,
		0x64, 0x66, 0x33, 0x63, 0x33, 0x30, 0x64, 0x36, 0x61, 0x38, 0x20, 0x38, 0x20, 0x38, 0x0A, 0x09,
		0x34, 0x35, 0x0D, 0x0A, 0x64, 0x37, 0x37, 0x32, 0x62, 0x31, 0x38, 0x39, 0x66, 0x31, 0x36, 0x34,
		0x35, 0x39, 0x35, 0x65, 0x32, 0x39, 0x65, 0x61, 0x38, 0x62, 0x63, 0x30, 0x32, 0x36, 0x37, 0x36,
		0x62, 0x66, 0x64, 0x66, 0x33, 0x63, 0x33, 0x30, 0x64, 0x36, 0x61, 0x38, 0x20, 0x39, 0x20, 0x39,
		0x0A, 0x09, 0xC3, 0xA4, 0xC3, 0xBC, 0x0A,
	};

	BYTE_VECTOR blame_output;
	EXPECT_EQ(0, blame_output.append(inputByteArray, sizeof(inputByteArray)));
	EXPECT_EQ(sizeof(inputByteArray), blame_output.size());

	CTortoiseGitBlameData data;
	CGitHashMap hashToRev;
	data.ParseBlameOutput(blame_output, hashToRev, 0, false);

	EXPECT_EQ(9, data.GetNumberOfLines());

	for (int i = 0; i < 9; ++i)
		EXPECT_TRUE(data.IsValidLine(i));
	EXPECT_FALSE(data.IsValidLine(9));

	for (int i = 0; i < 9; ++i)
		EXPECT_STREQ(L"Sven Strickroth", data.GetAuthor(i)); // not optimal since all lines have same author

	const CString hashes[] = { L"3d3420b", L"d772b18", L"d772b18", L"d772b18", L"6fcf6d7", L"d772b18", L"d772b18", L"d772b18", L"d772b18" };
	for (int i = 0; i < 9; ++i)
		EXPECT_STREQ(hashes[i], data.GetHash(i).ToString().Left(7));

	const CString dates[] = { L"2013-10-26 17:02:25", L"2013-10-26 16:59:50", L"2013-10-26 16:59:50", L"2013-10-26 16:59:50", L"2013-10-26 17:00:23", L"2013-10-26 16:59:50", L"2013-10-26 16:59:50", L"2013-10-26 16:59:50", L"2013-10-26 16:59:50" };
	for (int i = 0; i < 9; ++i)
		EXPECT_STREQ(dates[i], data.GetDate(i));

	data.UpdateEncoding(CP_UTF8);
	const CString lines[] = { L"ä#äf34öööäß€9875oe", L"fgdjkglsfdg", L"öäöü45g", L"fdgi&§$%&hfdsgä", L"ä#äf34öööäß€9875oe", L"öäüpfgmfdg", L"€fgfdsg", L"45", L"äü" };
	for (int i = 0; i < 9; ++i)
		EXPECT_STREQ(lines[i] + _T('\r'), CUnicodeUtils::GetUnicode(data.GetUtf8Line(i)));

	const int lineNumbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (int i = 0; i < 9; ++i)
		EXPECT_EQ(lineNumbers[i], data.GetOriginalLineNumber(i));

	for (int i = 0; i < 9; ++i)
		EXPECT_STREQ(L"utf-8.txt", data.GetFilename(i));
}

// add tests for real utf8 file w/o bom, utf16be, utf16le, utf32be, utf32le
// with different filename
