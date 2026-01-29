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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char sample[] = "{\"alpha\": 1, \"beta\": [1,2,3]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(sample, sizeof(sample) - 1, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *raw = cJSON_CreateRaw("{\"raw_inner\":true}");
    char *printed = NULL;
    char *printed_buf = NULL;
    char *prealloc_buf = (char *)cJSON_malloc(256);
    memset(prealloc_buf, 0, 256);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON_SetNumberHelper(num, 99.5);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    printed_buf = cJSON_PrintBuffered(root, 64, 0);
    cJSON_PrintPreallocated(root, prealloc_buf, 256, 1);
    cJSON *retrieved = cJSON_GetObjectItem(root, "number");
    double value_before = cJSON_GetNumberValue(retrieved);
    double value_after = cJSON_SetNumberHelper(retrieved, value_before + 0.25);
    (void)value_after;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_buf);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}