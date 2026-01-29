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
//<ID> 1511
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
    cJSON *middlename_null = cJSON_CreateNull();
    cJSON_AddItemToObject(person, "middlename", middlename_null);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(90.0));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(95.5));
    cJSON_AddItemToObject(person, "scores", scores);
    cJSON *location = cJSON_Parse("{\"city\":\"NY\",\"zip\":10001}");
    cJSON_AddItemToObject(root, "location", location);
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON_bool has_age = cJSON_HasObjectItem(person, "age");
    cJSON *mid_item = cJSON_GetObjectItem(person, "middlename");
    cJSON_bool mid_is_null = cJSON_IsNull(mid_item);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObjectCaseSensitive(person, "age");
    cJSON *age_after = cJSON_GetObjectItem(person, "age");
    cJSON_bool age_is_null_after_delete = cJSON_IsNull(age_after);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON *loc_zip_item = cJSON_GetObjectItem(location, "zip");
    double zip_value = cJSON_GetNumberValue(loc_zip_item);
    cJSON *loc_city_item = cJSON_GetObjectItem(location, "city");
    const char *city_value = cJSON_GetStringValue(loc_city_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)has_person;
    (void)has_age;
    (void)mid_is_null;
    (void)age_is_null_after_delete;
    (void)equal;
    (void)zip_value;
    (void)city_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}