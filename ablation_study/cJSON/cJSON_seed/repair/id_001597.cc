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
//<ID> 1597
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
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON *age_ref_item = cJSON_CreateNumber(30.0);
    cJSON_AddItemReferenceToObject(person, "age_ref", age_ref_item);
    cJSON_AddNumberToObject(person, "age_direct", 30.0);

    // step 2: Configure
    cJSON *person_ref = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(person_ref, "name");
    cJSON *got_age_ref = cJSON_GetObjectItemCaseSensitive(person_ref, "age_ref");
    cJSON *got_age_direct = cJSON_GetObjectItemCaseSensitive(person_ref, "age_direct");

    // step 3: Operate and Validate
    double age_ref_val = cJSON_GetNumberValue(got_age_ref);
    double age_direct_val = cJSON_GetNumberValue(got_age_direct);
    double sum_age = age_ref_val + age_direct_val;
    cJSON_AddNumberToObject(root, "sum_age", sum_age);
    cJSON *double_age = cJSON_CreateNumber(age_ref_val * 2.0);
    cJSON_AddItemToObject(root, "double_age", double_age);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    if (out && out_len) scratch[0] = out[0];
    scratch[1] = (char)('0' + (int)sum_age % 10);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}