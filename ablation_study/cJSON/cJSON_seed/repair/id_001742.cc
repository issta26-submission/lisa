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
//<ID> 1742
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
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "age", age);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(person, "active", active);
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToObject(person, "tags", tags);
    cJSON *tag1 = cJSON_CreateString("tester");
    cJSON_AddItemToArray(tags, tag1);

    // step 2: Configure
    double new_age = cJSON_SetNumberHelper(age, 31.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "configured");
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    cJSON *retr_person = cJSON_GetObjectItem(root, "person");
    cJSON *retr_name = cJSON_GetObjectItem(retr_person, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    double age_val = cJSON_GetNumberValue(cJSON_GetObjectItem(retr_person, "age"));
    cJSON_bool invalid_flag = cJSON_IsInvalid(retr_name);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version ? version[0] : 'V';
    buffer[2] = (char)('0' + ((int)new_age % 10));
    buffer[3] = (char)('0' + (int)invalid_flag);
    (void)name_val;
    (void)age_val;

    cJSON *detached_active = cJSON_DetachItemFromObject(retr_person, "active");
    cJSON_Delete(detached_active);
    cJSON *detached_name = cJSON_DetachItemViaPointer(retr_person, retr_name);
    cJSON_Delete(detached_name);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}