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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char sample_json[] = "{\"name\":\"alpha\",\"value\":0}";
    const size_t sample_len = sizeof(sample_json) - 1;
    const char *parse_end = NULL;
    char *printed = NULL;
    char *tmpbuf = (char *)cJSON_malloc(128);
    memset(tmpbuf, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(1.0);
    int ints_data[3] = {5, 6, 7};
    cJSON *ints_array = cJSON_CreateIntArray(ints_data, 3);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawkey\":true}");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "ints", ints_array);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "num", num_item);
    double set_result = cJSON_SetNumberHelper(num_item, 42.42);
    (void)set_result;

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    /* Ensure the buffer_length includes the terminating null when require_null_terminated is true */
    cJSON *parsed = cJSON_ParseWithLengthOpts(printed, (size_t)strlen(printed) + 1, &parse_end, 1);
    cJSON *parsed_num = cJSON_GetObjectItem(parsed, "num");
    double parsed_value = cJSON_GetNumberValue(parsed_num);
    double new_value = cJSON_SetNumberHelper(parsed_num, parsed_value + 1.0);
    (void)new_value;
    (void)parse_end;
    (void)parsed_value;

    // step 4: Cleanup
    cJSON_free(tmpbuf);
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}