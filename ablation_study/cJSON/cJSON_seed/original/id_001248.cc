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
//<ID> 1248
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddItemToObject(inner, "inner_key", cJSON_CreateString("inner_value"));
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num);

    cJSON *inner_ref = cJSON_CreateObjectReference(inner);
    cJSON_AddItemToObject(root, "inner_ref", inner_ref);
    cJSON_bool inner_is_obj = cJSON_IsObject(inner);
    cJSON_AddBoolToObject(root, "inner_is_object", inner_is_obj);

    int buf_size = 1024;
    char *buf = (char *)cJSON_malloc((size_t)buf_size);
    memset(buf, 0, (size_t)buf_size);
    cJSON_PrintPreallocated(root, buf, buf_size, 0);
    char *printed = buf;
    size_t printed_len = strlen(printed);
    cJSON_Minify(buf);
    cJSON *got_inner = cJSON_GetObjectItem(root, "inner");
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    cJSON *pi_times_two = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "pi_twice", pi_times_two);

    cJSON *ref_item = cJSON_GetObjectItem(root, "inner_ref");
    cJSON *ref_inner_key = cJSON_GetObjectItem(ref_item, "inner_key");
    const char *recovered = cJSON_GetStringValue(ref_inner_key);
    cJSON_AddStringToObject(root, "recovered_inner_key", recovered);
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
}