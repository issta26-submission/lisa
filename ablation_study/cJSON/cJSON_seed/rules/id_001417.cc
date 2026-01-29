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
//<ID> 1417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *node1 = NULL;
    cJSON *node2 = NULL;
    cJSON *node3 = NULL;
    char *printed = NULL;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    const char *json_literal1 = "{\"x\":10,\"y\":\"text\"}";
    const char *json_literal2 = "{\"z\":[1,2,3]}";
    char mutable_json[] = "  { \"a\" : 42 , \"b\" : \"hello\" }  ";
    int prebuffer = 128;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    node1 = cJSON_Parse(json_literal1);
    node2 = cJSON_ParseWithLength(json_literal2, (size_t)13);
    cJSON_Minify(mutable_json);
    node3 = cJSON_Parse(mutable_json);
    added1 = cJSON_AddItemToObject(root, "first", node1);
    added2 = cJSON_AddItemToObject(root, "second", node2);
    added3 = cJSON_AddItemToObject(root, "third", node3);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered((const cJSON *)root, prebuffer, 1);
    (void)added1;
    (void)added2;
    (void)added3;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}