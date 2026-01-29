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
//<ID> 1220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *n1 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(items, n1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("cfg");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *factor = cJSON_CreateNumber(1.5);
    cJSON_AddItemToObject(meta, "factor", factor);

    // step 3: Operate and Validate
    cJSON_bool s1_is_string = cJSON_IsString(s1);
    double v_n1 = cJSON_GetNumberValue(n1);
    cJSON *factor_ptr = cJSON_GetObjectItem(meta, "factor");
    double v_factor = cJSON_GetNumberValue(factor_ptr);
    double computed = v_n1 * v_factor + (double)s1_is_string;
    cJSON *final_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "final", final_num);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_items);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(8);
    memset(scratch, 0, 8);
    ((char *)scratch)[0] = out[0];
    unsigned char first_byte = (unsigned char)((char *)scratch)[0];
    double augmented = computed + (double)first_byte;
    cJSON *aug_num = cJSON_CreateNumber(augmented);
    cJSON_AddItemToObject(root, "augmented", aug_num);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}