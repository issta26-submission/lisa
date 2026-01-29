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
//<ID> 690
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
    const char *jsonA = "{\"name\":\"tester\",\"value\":123}";
    const char *jsonB = "{\"title\":\"Sample\"}";
    const char *endA = NULL;
    const char *endB = NULL;
    size_t printed_len = 0;
    cJSON *parsedA = cJSON_ParseWithLengthOpts(jsonA, strlen(jsonA), &endA, 1);
    cJSON *parsedB = cJSON_ParseWithOpts(jsonB, &endB, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON *name_item = cJSON_GetObjectItem(parsedA, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_string);
    cJSON *title_item = cJSON_GetObjectItem(parsedB, "title");
    char *title_str = cJSON_GetStringValue(title_item);
    cJSON_AddStringToObject(root, "title_copy", title_str);

    // step 3: Operate / Validate
    cJSON_PrintPreallocated(root, scratch, 256, 0);
    printed_len = strlen(scratch);
    const char *endC = NULL;
    cJSON *reparsed = cJSON_ParseWithLengthOpts(scratch, printed_len, &endC, 0);
    cJSON_bool title_is_string_after = cJSON_IsString(cJSON_GetObjectItem(reparsed, "title_copy"));
    cJSON_AddBoolToObject(root, "title_is_string_after_roundtrip", title_is_string_after);

    // step 4: Cleanup
    cJSON_Delete(parsedA);
    cJSON_Delete(parsedB);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}