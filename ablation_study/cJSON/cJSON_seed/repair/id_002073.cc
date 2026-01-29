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
//<ID> 2073
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
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON *hobbies = cJSON_CreateArray();
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("reading"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("jogging"));
    cJSON_AddItemToObject(person, "hobbies", hobbies);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    const char config_json[] = "{\"message\":\"hello\",\"pi\":3.1415}";
    cJSON *parsed = cJSON_Parse(config_json);
    cJSON *msg = cJSON_GetObjectItem(parsed, "message");
    const char *msg_str = cJSON_GetStringValue(msg);
    cJSON *pi_item = cJSON_GetObjectItem(parsed, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON_AddItemToObject(root, "msg_copy", cJSON_CreateString(msg_str));
    cJSON_AddItemToObject(root, "pi_copy", cJSON_CreateNumber(pi_val));

    // step 3: Operate and Validate
    cJSON *got_person = cJSON_GetObjectItem(root, "person");
    cJSON *got_name = cJSON_GetObjectItem(got_person, "name");
    const char *got_name_str = cJSON_GetStringValue(got_name);
    cJSON *got_age = cJSON_GetObjectItem(got_person, "age");
    double got_age_val = cJSON_GetNumberValue(got_age);
    cJSON *person_dup = cJSON_Duplicate(got_person, 1);
    cJSON_bool same = cJSON_Compare(got_person, person_dup, 1);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = got_name_str[0];
    buffer[2] = (char)('0' + ((int)got_age_val % 10));
    buffer[3] = (char)(same ? '1' : '0');
    buffer[4] = (char)('0' + (int)((int)pi_val % 10));
    buffer[5] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(person_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}