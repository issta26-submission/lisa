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
//<ID> 697
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printbuf = (char *)cJSON_malloc(256);
    memset(printbuf, 0, 256);
    const char json[] = "{\"name\":\"tester\",\"value\":123}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end1 = NULL;
    const char *parse_end2 = NULL;
    cJSON *parsed1 = cJSON_ParseWithLengthOpts(json, json_len, &parse_end1, 1);
    cJSON *parsed2 = cJSON_ParseWithOpts(json, &parse_end2, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *dup_parsed2 = NULL;
    cJSON_bool name_is_string = 0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "first_parsed", parsed1);
    dup_parsed2 = cJSON_Duplicate(parsed2, 1);
    cJSON_AddItemToObject(root, "second_parsed_copy", dup_parsed2);

    // step 3: Operate / Validate
    name_is_string = cJSON_IsString(cJSON_GetObjectItem(parsed2, "name"));
    cJSON_AddBoolToObject(root, "name_is_string", name_is_string);
    printed_ok = cJSON_PrintPreallocated(root, printbuf, 256, 0);

    // step 4: Cleanup
    cJSON_Delete(parsed2);
    cJSON_Delete(root);
    cJSON_free(printbuf);

    // API sequence test completed successfully
    return 66;
}