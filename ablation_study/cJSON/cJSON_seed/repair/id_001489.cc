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
//<ID> 1489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1,2,3],\"info\":{\"x\":42.5}}";
    const char *parse_end = NULL;
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);

    // step 2: Configure
    cJSON *info = cJSON_GetObjectItemCaseSensitive(root, "info");
    cJSON *x_item = cJSON_GetObjectItemCaseSensitive(info, "x");
    double x_val = cJSON_GetNumberValue(x_item);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON_DeleteItemFromArray(arr, 1);

    // step 3: Operate and Validate
    cJSON_AddNumberToObject(new_obj, "extracted_x", x_val);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = 'A';
    scratch[1] = (char)('0' + ((int)x_val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    double recorded = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(new_obj, "extracted_x"));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(new_obj);
    // API sequence test completed successfully
    return 66;
}