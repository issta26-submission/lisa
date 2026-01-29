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
//<ID> 1486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"values\":[10,20,30],\"meta\":{\"factor\":2.5}}";
    const char *parse_end = NULL;
    size_t json_len = (size_t)strlen(json);
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *factor_item = cJSON_GetObjectItemCaseSensitive(meta, "factor");

    // step 2: Configure
    cJSON *result = cJSON_CreateObject();
    cJSON *first_val = cJSON_GetArrayItem(values, 0);
    double first_num = cJSON_GetNumberValue(first_val);
    double factor = cJSON_GetNumberValue(factor_item);
    double computed = first_num * factor;
    cJSON_AddNumberToObject(result, "computed", computed);
    cJSON_AddItemToObject(root, "result", result);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(values, 1);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)((int)computed % 10));
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('A');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}