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
//<ID> 1772
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
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 29.0);
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToObject(person, "scores", scores);
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(88.5));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(92.0));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(75.25));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "source", "unit-test");
    cJSON_AddStringToObject(root, "cjson_version", cJSON_Version());

    // step 3: Operate and Validate
    cJSON *dup_person = cJSON_Duplicate(person, 1);
    cJSON_bool same = cJSON_Compare(person, dup_person, 1);
    char *printed = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    cJSON *age_item = cJSON_GetObjectItem(person, "age");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    char *name_val = cJSON_GetStringValue(name_item);
    double age_val = cJSON_GetNumberValue(age_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)name_is_string);
    int age_int = (int)age_val;
    buffer[2] = (char)('0' + (age_int % 10));
    buffer[3] = name_val ? name_val[0] : 'Z';
    (void)buffer;
    (void)age_int;
    (void)same;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(pretty);
    cJSON_free(buffer);
    cJSON_Delete(dup_person);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}