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
//<ID> 1415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item1 = NULL;
    cJSON *num_item2 = NULL;
    cJSON *parsed1 = NULL;
    cJSON *parsed2 = NULL;
    cJSON *dup_parsed2 = NULL;
    char *printed = NULL;
    char json1[] = "{\"alpha\":true,\"beta\":[10,20]}";
    char json2[] = "{\n  \"gamma\": \"value\",\n  \"delta\": 3.14\n}";
    char minify_buf[] = " { \"to_minify\" : [ 1, 2 , 3 ] } ";
    size_t json2_len = sizeof(json2) - 1;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item1 = cJSON_CreateNumber(10.0);
    num_item2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num_item1);
    cJSON_AddItemToArray(arr, num_item2);
    cJSON_AddItemToObject(root, "arr", arr);
    parsed1 = cJSON_Parse(json1);
    cJSON_AddItemToObject(root, "parsed1", parsed1);
    parsed2 = cJSON_ParseWithLength(json2, json2_len);
    dup_parsed2 = cJSON_Duplicate(parsed2, 1);
    cJSON_AddItemToObject(root, "parsed2dup", dup_parsed2);

    // step 3: Operate / Validate
    cJSON_Minify(minify_buf);
    printed = cJSON_PrintBuffered(root, 128, 1);
    (void)minify_buf;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed2);
    // API sequence test completed successfully
    return 66;
}