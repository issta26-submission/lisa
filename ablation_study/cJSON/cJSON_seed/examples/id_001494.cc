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
//<ID> 1494
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
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 29.0);
    cJSON_AddItemToObject(root, "original", person);

    // Create a members array and add a duplicate of 'person' into it.
    // Using a duplicate avoids ownership/reference issues that can cause double-free on cleanup.
    cJSON *members = cJSON_CreateArray();
    cJSON *member_copy = cJSON_Duplicate(person, 1);
    cJSON_AddItemToArray(members, member_copy);
    cJSON_AddItemToObject(root, "members", members);

    // step 2: Configure
    char *printed_root = cJSON_PrintUnformatted(root);
    void *temp_buf = cJSON_malloc(128);
    memset(temp_buf, 0, 128);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    (void)root_is_obj;

    // step 3: Operate & Validate
    cJSON *members_got = cJSON_GetObjectItem(root, "members");
    cJSON *first = cJSON_GetArrayItem(members_got, 0);
    cJSON *name_item = cJSON_GetObjectItem(first, "name");
    const char *name_value = cJSON_GetStringValue(name_item);
    cJSON *mirror = cJSON_CreateObject();
    cJSON_AddStringToObject(mirror, "copied_name", name_value);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    (void)equal;
    cJSON *copied_name_item = cJSON_GetObjectItem(mirror, "copied_name");
    const char *copied_name_value = cJSON_GetStringValue(copied_name_item);
    (void)copied_name_value;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(temp_buf);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    cJSON_Delete(mirror);

    // API sequence test completed successfully
    return 66;
}