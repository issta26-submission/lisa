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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    char *printed = NULL;
    char *minbuf = NULL;
    char *printed2 = NULL;
    cJSON *num_item = NULL;
    cJSON *added_item = NULL;
    cJSON *replaced_item = NULL;
    double num_value = 0.0;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "num", 42.5);
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    minbuf = (char *)cJSON_malloc(printed_len + 1);
    memcpy(minbuf, printed, printed_len + 1);
    cJSON_Minify(minbuf);
    parsed = cJSON_Parse(minbuf);

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(parsed, "num");
    num_value = cJSON_GetNumberValue(num_item);
    added_item = cJSON_AddNumberToObject(parsed, "added", num_value + 10.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "num", cJSON_CreateNumber(100.0));
    replaced_item = cJSON_GetObjectItem(parsed, "num");
    cJSON_AddTrueToObject(parsed, "enabled");
    printed2 = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(minbuf);
    cJSON_free(printed2);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}