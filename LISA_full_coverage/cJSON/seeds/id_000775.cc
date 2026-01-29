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
//<ID> 775
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = "{ \"arr\": [10, 20, 30], \"name\": \"orig\" }";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, (size_t)(sizeof(json_text) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw", "{\"inner\":123}");
    cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}