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
//<ID> 1910
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
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "age", age);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name);

    // step 2: Configure
    cJSON *new_age = cJSON_CreateNumber(31.0);
    cJSON_ReplaceItemInObject(person, "age", new_age);
    char *setname = cJSON_SetValuestring(name, "Bob");
    cJSON *status = cJSON_CreateString("active");
    cJSON_AddItemToObject(person, "status", status);

    // step 3: Operate and Validate
    cJSON *got_age = cJSON_GetObjectItem(person, "age");
    double age_val = cJSON_GetNumberValue(got_age);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = setname ? setname[0] : 'X';
    buffer[2] = (char)('0' + ((int)age_val % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}