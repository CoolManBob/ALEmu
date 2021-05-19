-- MySQL dump 10.13  Distrib 8.0.20, for Win64 (x86_64)
--
-- Host: localhost    Database: alemuworld
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
-- Table structure for table `character_data`
--

DROP TABLE IF EXISTS `character_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_data` (
  `charID` int NOT NULL AUTO_INCREMENT,
  `acctID` int NOT NULL,
  `charName` varchar(49) COLLATE utf8mb4_general_ci NOT NULL DEFAULT 'Unknown',
  `charTID` int NOT NULL,
  `slot` int NOT NULL DEFAULT '0',
  `posX` double DEFAULT '0',
  `posY` double DEFAULT '0',
  `posZ` double DEFAULT '0',
  `HP` int DEFAULT '1',
  `MP` int DEFAULT '1',
  `SP` int DEFAULT '1',
  `EXP` int DEFAULT '1',
  `level` int DEFAULT '1',
  `skillpoint` int DEFAULT '0',
  `invenmoney` int DEFAULT '0',
  `hair` int DEFAULT '0',
  `face` int DEFAULT '0',
  PRIMARY KEY (`charID`),
  UNIQUE KEY `charName_UNIQUE` (`charName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_data`
--

LOCK TABLES `character_data` WRITE;
/*!40000 ALTER TABLE `character_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `character_inventory`
--

DROP TABLE IF EXISTS `character_inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `character_inventory` (
  `char_id` int NOT NULL,
  PRIMARY KEY (`char_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `character_inventory`
--

LOCK TABLES `character_inventory` WRITE;
/*!40000 ALTER TABLE `character_inventory` DISABLE KEYS */;
/*!40000 ALTER TABLE `character_inventory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'alemuworld'
--

--
-- Dumping routines for database 'alemuworld'
--
/*!50003 DROP PROCEDURE IF EXISTS `createCharacter` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `createCharacter`(
					IN ACCTID INT,
                    IN TID INT,
                    IN FACE INT,
                    IN HAIR INT,
                    IN CHARNAME VARCHAR(49))
BEGIN
	DECLARE varslot INT DEFAULT 0;
    SELECT MAX( slot ) INTO varslot FROM character_data WHERE acctID = ACCTID;
    IF varslot IS NULL THEN
		SET varslot = -1; -- This allows slot indexing to start at 0
	END IF;
	INSERT INTO character_data (acctID, charName, slot, charTID, face, hair) VALUES (ACCTID,CHARNAME,varslot+1,TID,FACE,HAIR);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-05-19 12:13:06
