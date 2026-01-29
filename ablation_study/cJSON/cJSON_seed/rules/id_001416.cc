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
//<ID> 1416
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
    cJSON *parsed_from_print = NULL;
    char *printed = NULL;
    char json1[] = "{\"name\":\"Alice\",\"age\":30}";
    char mutable_json[] = "  { \"compact\": true , \"vals\": [ 1 , 2 ] }  ";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed1 = cJSON_Parse(json1);
    cJSON_AddItemToObject(root, "profile", parsed1);
    cJSON_Minify(mutable_json);
    parsed2 = cJSON_ParseWithLength(mutable_json, (size_t)(sizeof(mutable_json) - 1));
    cJSON_AddItemToObject(root, "compact_obj", parsed2);
    cJSON_AddItemToObject(root, "score", cJSON_CreateNumber(99.5));

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 128, 1);
    parsed_from_print = cJSON_Parse(printed);

    // step 4: Cleanup
    cJSON_Delete(parsed_from_print);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}