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
//<ID> 1414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed_obj = NULL;
    cJSON *parsed_arr = NULL;
    char *printed = NULL;
    char json_obj[] = "{ \"name\": \"example\", \"value\": 123 }";
    char json_arr[] = "[ 1, 2, 3, 4 ]";
    char minify_buf[] = "  {  \"whitespace\" : true, \"n\":  42 }  ";

    // step 2: Setup / Configure
    parsed_obj = cJSON_Parse(json_obj);
    parsed_arr = cJSON_ParseWithLength(json_arr, (size_t)(sizeof(json_arr) - 1));
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "object_from_parse", parsed_obj);
    cJSON_AddItemToObject(root, "array_from_parse", parsed_arr);

    // step 3: Operate / Validate
    cJSON_Minify(minify_buf);
    printed = cJSON_PrintBuffered(root, 256, 1);
    (void)minify_buf;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}