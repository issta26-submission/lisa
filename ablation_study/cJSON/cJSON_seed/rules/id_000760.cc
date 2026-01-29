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
//<ID> 760
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *inner = NULL;
    char *printed = NULL;
    double read_number = 0.0;
    double inner_value = 0.0;
    size_t json_len = 0;

    // step 2: Setup / Configure
    memcpy(scratch, "{\"init\": 10}", 13);
    json_len = strlen(scratch);
    parsed = cJSON_ParseWithLengthOpts(scratch, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddNumberToObject(root, "added", 3.14);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateFalse());

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root, "added");
    read_number = cJSON_GetNumberValue(num_item);
    inner = cJSON_GetObjectItem(parsed, "init");
    inner_value = cJSON_GetNumberValue(inner);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}