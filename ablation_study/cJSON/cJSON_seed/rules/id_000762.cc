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
//<ID> 762
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    const char *parse_end = NULL;
    const char *json_text = "{\"n\": 3.14}";
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *false_item = NULL;
    char *printed = NULL;
    double extracted_value = 0.0;
    const cJSON *n_item = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json_text, strlen(json_text), &parse_end, 1);
    root = cJSON_CreateObject();
    extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "n"));
    cJSON_AddNumberToObject(root, "copied", extracted_value);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 3: Operate / Validate
    n_item = cJSON_GetObjectItem(root, "copied");
    extracted_value = cJSON_GetNumberValue(n_item);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}