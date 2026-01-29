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
//<ID> 1129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON *n2 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "scale", 1.5);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "note", note);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *meta_ref = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta_ref, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    cJSON *numbers_ref = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON *first_item = cJSON_GetArrayItem(numbers_ref, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double computed_val = first_val * scale;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}