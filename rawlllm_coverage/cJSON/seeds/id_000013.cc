#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Add simple members
    if (!cJSON_AddStringToObject(root, "name", "Alice")) {
        cJSON_Delete(root);
        return 2;
    }
    if (!cJSON_AddNumberToObject(root, "age", 30)) {
        cJSON_Delete(root);
        return 3;
    }

    // Create an array and add several string elements, then attach to root
    cJSON *skills = cJSON_CreateArray();
    if (!skills) {
        cJSON_Delete(root);
        return 4;
    }
    if (!cJSON_AddItemToArray(skills, cJSON_CreateString("C"))) {
        cJSON_Delete(skills);
        cJSON_Delete(root);
        return 5;
    }
    if (!cJSON_AddItemToArray(skills, cJSON_CreateString("C++"))) {
        cJSON_Delete(skills);
        cJSON_Delete(root);
        return 6;
    }
    if (!cJSON_AddItemToArray(skills, cJSON_CreateString("Python"))) {
        cJSON_Delete(skills);
        cJSON_Delete(root);
        return 7;
    }
    // Attach skills array to root
    cJSON_AddItemToObject(root, "skills", skills);

    // Print the JSON (compact)
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) {
        cJSON_Delete(root);
        return 8;
    }

    // Parse the printed JSON back
    cJSON *parsed = cJSON_Parse(printed);
    free(printed); // printed string no longer needed
    if (!parsed) {
        cJSON_Delete(root);
        return 9;
    }

    // Validate parsed contents
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || name->valuestring == NULL) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 10;
    }
    if (std::strcmp(name->valuestring, "Alice") != 0) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 11;
    }

    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 12;
    }
    if (age->valuedouble != 30) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 13;
    }

    cJSON *parsed_skills = cJSON_GetObjectItemCaseSensitive(parsed, "skills");
    if (!parsed_skills || !cJSON_IsArray(parsed_skills)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 14;
    }
    int skills_count = cJSON_GetArraySize(parsed_skills);
    if (skills_count != 3) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 15;
    }
    // Verify each skill string
    const char *expected[] = { "C", "C++", "Python" };
    for (int i = 0; i < skills_count; ++i) {
        cJSON *item = cJSON_GetArrayItem(parsed_skills, i);
        if (!cJSON_IsString(item) || item->valuestring == NULL) {
            cJSON_Delete(parsed);
            cJSON_Delete(root);
            return 16 + i;
        }
        if (std::strcmp(item->valuestring, expected[i]) != 0) {
            cJSON_Delete(parsed);
            cJSON_Delete(root);
            return 20 + i;
        }
    }

    // Duplicate the parsed root (deep copy) and modify the duplicate
    cJSON *dup = cJSON_Duplicate(parsed, /*recurse=*/1);
    if (!dup) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 23;
    }
    // Add a new field to the duplicate
    if (!cJSON_AddStringToObject(dup, "city", "Wonderland")) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 24;
    }
    // Remove 'age' from duplicate and verify removal
    cJSON_DeleteItemFromObjectCaseSensitive(dup, "age");
    if (cJSON_GetObjectItemCaseSensitive(dup, "age") != NULL) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 25;
    }

    // Cleanup all created JSON structures
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If all operations succeeded, return the required success value
    return 66;
}