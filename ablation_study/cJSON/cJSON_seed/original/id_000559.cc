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
//<ID> 559
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"name\" : \"tester\", \"value\" : 0, \"flag\" : true } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON_SetNumberHelper(value_item, 42.5);
    cJSON_AddTrueToObject(root, "confirmed");

    // step 3: Operate
    char *printed = cJSON_Print(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t scratch_len = printed_len + 32;
    void *scratch = cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    strcpy((char *)scratch, printed);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)extracted;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}