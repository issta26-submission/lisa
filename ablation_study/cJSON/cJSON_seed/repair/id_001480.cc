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
//<ID> 1480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"numbers\":[1,2,3,4],\"meta\":{\"scale\":2.5}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)strlen(json), &parse_end, 1);
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");

    // step 2: Configure
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON_DeleteItemFromArray(numbers, 1);
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    cJSON_AddNumberToObject(summary, "scale_doubled", scale * 2.0);

    // step 3: Operate and Validate
    cJSON *first_num = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first_num);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : 'X';
    scratch[1] = (char)('0' + ((int)first_val % 10));
    scratch[2] = (char)('0' + ((int)scale % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}