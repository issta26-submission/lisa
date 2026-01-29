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
//<ID> 1590
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
    cJSON *name_person = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name_person);
    cJSON *name_ref = cJSON_CreateString("Alice");
    cJSON_AddItemReferenceToObject(root, "name_ref", name_ref);
    cJSON *age_item = cJSON_AddNumberToObject(person, "age", 30.0);
    cJSON *bonus = cJSON_CreateNumber(5.5);
    cJSON_AddItemToObject(person, "bonus", bonus);

    // step 2: Configure
    cJSON *person_ref = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *age_ref = cJSON_GetObjectItemCaseSensitive(person_ref, "age");
    cJSON *name_ref_get = cJSON_GetObjectItemCaseSensitive(root, "name_ref");

    // step 3: Operate and Validate
    double age_val = cJSON_GetNumberValue(age_ref);
    cJSON *double_age = cJSON_CreateNumber(age_val * 2.0);
    cJSON_AddItemToObject(root, "double_age", double_age);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (int)age_val);
    scratch[2] = name_ref_get && name_ref_get->valuestring && name_ref_get->valuestring[0] ? name_ref_get->valuestring[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}