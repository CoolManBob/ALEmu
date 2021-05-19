-- MySQL dump 10.13  Distrib 8.0.20, for Win64 (x86_64)
--
-- Host: localhost    Database: alemulogin
-- ------------------------------------------------------
-- Server version	8.0.20

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `account` (
  `acctID` int NOT NULL AUTO_INCREMENT,
  `acctName` varchar(45) DEFAULT NULL,
  `acctPW` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`acctID`),
  UNIQUE KEY `name_UNIQUE` (`acctName`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `account`
--

LOCK TABLES `account` WRITE;
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
INSERT INTO `account` VALUES (1,'test','test'),(2,'test2','9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08');
/*!40000 ALTER TABLE `account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `worldgroup`
--

DROP TABLE IF EXISTS `worldgroup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `worldgroup` (
  `groupID` int NOT NULL,
  `groupName` varchar(45) NOT NULL,
  PRIMARY KEY (`groupID`),
  UNIQUE KEY `groupName_UNIQUE` (`groupName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `worldgroup`
--

LOCK TABLES `worldgroup` WRITE;
/*!40000 ALTER TABLE `worldgroup` DISABLE KEYS */;
INSERT INTO `worldgroup` VALUES (1,'Group1'),(2,'Group2');
/*!40000 ALTER TABLE `worldgroup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `worldlist`
--

DROP TABLE IF EXISTS `worldlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `worldlist` (
  `worldID` int NOT NULL,
  `groupID` int NOT NULL,
  `worldName` varchar(45) NOT NULL,
  `worldAddress` varchar(45) NOT NULL,
  `worldStatus` int unsigned NOT NULL DEFAULT '0',
  `worldFlag` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`worldID`),
  UNIQUE KEY `worldName_UNIQUE` (`worldName`),
  UNIQUE KEY `worldAddress_UNIQUE` (`worldAddress`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `worldlist`
--

LOCK TABLES `worldlist` WRITE;
/*!40000 ALTER TABLE `worldlist` DISABLE KEYS */;
INSERT INTO `worldlist` VALUES (1,1,'ALEmu','127.0.0.1:11008',4,4),(2,2,'ALEmu2','127.0.0.1:11009',1,1),(3,1,'ALEmuTest','127.0.0.1:11010',1,1);
/*!40000 ALTER TABLE `worldlist` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-12-14 11:13:35
