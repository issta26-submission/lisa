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
//<ID> 1513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON_AddStringToObject(person, "name", "Bob");
    cJSON_AddNumberToObject(person, "age", 28.0);
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(100.0));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(95.5));
    cJSON_AddItemToObject(person, "scores", scores);
    cJSON_AddItemToObject(root, "person", person);
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", missing);
    const char json_text[] = "{\"alpha\":1,\"beta\":null}";
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, (size_t)(sizeof(json_text) - 1), NULL, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *duplicate = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON *missing_item = cJSON_GetObjectItem(root, "missing");
    cJSON_bool missing_is_null = cJSON_IsNull(missing_item);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "parsed");
    cJSON_bool has_parsed_after_delete = cJSON_HasObjectItem(root, "parsed");
    (void)has_person;
    (void)missing_is_null;
    (void)equal;
    (void)has_parsed_after_delete;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}