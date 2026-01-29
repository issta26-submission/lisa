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
//<ID> 1411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed1 = NULL;
    cJSON *parsed2 = NULL;
    char *printed = NULL;
    char mutable_json[] = " {  \"name\" : \"Alice\" , \"age\" : 30 , \"items\" : [ 1 , 2 , 3 ] } ";
    size_t json_len = (size_t)(sizeof(mutable_json) - 1);

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_Minify(mutable_json);
    parsed1 = cJSON_Parse(mutable_json);
    parsed2 = cJSON_ParseWithLength(mutable_json, json_len);
    cJSON_AddItemToObject(root, "from_parse", parsed1);
    cJSON_AddItemToObject(root, "from_parse_with_length", parsed2);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}