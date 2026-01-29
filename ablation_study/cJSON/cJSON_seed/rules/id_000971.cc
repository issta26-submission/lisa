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
//<ID> 971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *added_obj = NULL;
    cJSON *value_item = NULL;
    cJSON *flags_arr = NULL;
    char *printed_unfmt = NULL;
    char *prebuf = NULL;
    const char *parse_end = NULL;
    const char *version = NULL;
    const char json[] = "{\"name\":\"unit\",\"value\":123.456,\"flags\":[true,false,null]}";
    size_t json_len = (size_t)(sizeof(json) - 1);
    int prebuf_len = 256;
    double value_num = 0.0;
    int intpart = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    added_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(added_obj, "added", 7.25);
    cJSON_AddItemToObject(root, "extra", added_obj);
    flags_arr = cJSON_GetObjectItem(root, "flags");

    // step 3: Operate / Validate
    printed_unfmt = cJSON_PrintUnformatted(root);
    prebuf = (char *)cJSON_malloc((size_t)prebuf_len);
    memset(prebuf, 0, (size_t)prebuf_len);
    cJSON_PrintPreallocated(root, prebuf, prebuf_len, 0);
    version = cJSON_Version();
    value_item = cJSON_GetObjectItem(root, "value");
    value_num = cJSON_GetNumberValue(value_item);
    intpart = (int)value_num;
    prebuf[0] = (char)((intpart % 10) + '0');
    prebuf[1] = version[0];
    prebuf[2] = (char)('0' + (flags_arr ? (flags_arr->type % 10) : 0));

    // step 4: Cleanup
    cJSON_free(printed_unfmt);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}