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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON_InsertItemInArray(values, 1, cJSON_CreateNumber(1.5));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "multiplier", 10.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    double computed = first_val + (double)flag_is_false * cJSON_GetNumberValue(cJSON_GetObjectItem(root, "meta")->child); /* uses multiplier stored in meta */
    cJSON_AddNumberToObject(root, "computed", computed);
    const char *raw = "{ \"x\" :  42 , \"y\" : [  7 ,8 ] }";
    size_t raw_len = strlen(raw) + 1;
    char *buf = (char *)cJSON_malloc(raw_len);
    memcpy(buf, raw, raw_len);
    cJSON_Minify(buf);
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}